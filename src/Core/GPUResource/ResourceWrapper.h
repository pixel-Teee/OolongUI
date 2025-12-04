#pragma once

namespace Oolong
{
	struct ResourceBase : public std::enable_shared_from_this<ResourceBase> {
		virtual ~ResourceBase() = default;
		virtual void scheduleForDeletion() = 0;

		//持有对其他资源的强引用
		std::vector<std::shared_ptr<ResourceBase>> dependencies;
	};

	class ResourceManager;
	template<typename ResourceType, typename Deleter>
	class ResourceWrapper
	{
	public:
		ResourceWrapper(ResourceType handle, ResourceManager* manager, uint64_t frameIndex)
			: m_handle(handle), m_manager(manager), m_createFrame(frameIndex)
		{}

		~ResourceWrapper() {
			//将资源放入待销毁队列
			if (m_manager && m_handle) {
				//这里根据 deleter 类型进行构造对象
				m_manager->scheduleForDeletion(m_handle, m_createFrame, Deleter{});
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
	};
}