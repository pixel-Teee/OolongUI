#pragma once

namespace Oolong
{
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
		}

		ResourceType get() const { return m_handle; }
	private:
		ResourceType m_handle;// SDL GPU Resource
		ResourceManager* m_manager;
		uint64_t m_createFrame;//资源创建的帧序号，用于垃圾回收
	};
}