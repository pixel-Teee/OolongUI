#pragma once

#include <GPUResource/DefaultDeleter.h>

#include <vector>

namespace Oolong
{
	struct ResourceBase : public std::enable_shared_from_this<ResourceBase> {
		virtual ~ResourceBase() = default;
		//持有对其他资源的强引用
		std::vector<std::shared_ptr<ResourceBase>> dependencies;
	};

	class ResourceManager;
	template<typename ResourceType, typename Deleter = DefaultDeleter<ResourceType>>
	class ResourceWrapper : public ResourceBase
	{
	public:
		ResourceWrapper(ResourceType handle, Deleter&& deleter, ResourceManager* manager, uint64_t frameIndex)
			: m_handle(handle), 
			  m_deleter(std::forward<Deleter>(deleter)), 
			  m_manager(manager), 
			  m_createFrame(frameIndex)
		{}

		virtual ~ResourceWrapper() {
			//将资源放入待销毁队列
			if (m_manager && m_handle) {
				//这里根据 deleter 类型进行构造对象
				m_manager->scheduleForDeletion(m_handle, m_createFrame, m_deleter);
			}

			//dependencies 会在这里析构
		}

		//添加一个依赖资源
		void addDependency(std::shared_ptr<ResourceBase> dependentResource)
		{
			dependencies.push_back(std::move(dependentResource));
		}

		ResourceType get() const { return m_handle; }
	private:
		ResourceType m_handle;// SDL GPU Resource
		ResourceManager* m_manager;
		uint64_t m_createFrame;//资源创建的帧序号，用于垃圾回收
		Deleter m_deleter;
	};

	template<typename ResourceType>
	using ResourceWrapperPtr = std::shared_ptr<ResourceWrapper<ResourceType, DefaultDeleter<ResourceType>>>;
}