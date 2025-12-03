#pragma once

#include <SDL3/SDL_gpu.h>
#include <vector>
#include <mutex>
#include "PendingDeletion.h"
#include "ResourceWrapper.h"

namespace Oolong
{
	class ResourceManager
	{
	public:
		ResourceManager(SDL_GPUDevice* device)
			: m_device(device)
			, m_currentFrame(0)
		{}

		template<typename ResourceType, typename Deleter>
		std::shared_ptr<ResourceWrapper<ResourceType, Deleter>>
			registerResource(ResourceType handle, Deleter deleter)
		{
			auto wrapper = std::make_shared<ResourceWrapper<ResourceType, Deleter>>
				(handle, this, m_currentFrame);
			return wrapper;
		}

		//每帧开始或结束的时候调用
		void nextFrame()
		{
			m_currentFrame++;
			//尝试清理N帧之前标记为待删除的资源
			tryCleanup();
		}

		template<typename ResourceType, typename Deleter>
		void scheduleForDeletion(ResourceType handle, uint64_t frameIndex, Deleter deleter)
		{
			auto deleterFunc = [this, handle, deleter]() {
				deleter(m_device, handle, nullptr); //调用真正的GPU销毁函数
			};

			std::lock_guard<std::mutex> lock(m_deletionMutex);
			m_pendingDeletions.emplace_back(deleterFunc, frameIndex);
		}

		void tryCleanup()
		{
			std::lock_guard<std::mutex> lock(m_deletionMutex);

			const uint64_t safeFrame = m_currentFrame > 3 ? m_currentFrame - 3 : 0;

			auto it = m_pendingDeletions.begin();
			while (it != m_pendingDeletions.end())
			{
				if (it->frameIndex < safeFrame)
				{
					//执行真正的GPU资源销毁
					it->deleter();
					it = m_pendingDeletions.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

		void shutDown()
		{
			SDL_WaitForGPUIdle(m_device);
			std::lock_guard<std::mutex> lock(m_deletionMutex);
			for (auto& pending : m_pendingDeletions) {
				pending.deleter();
			}
			m_pendingDeletions.clear();
		}
	private:
		SDL_GPUDevice* m_device;//device
		uint64_t m_currentFrame;//一直递增的帧计数器
		std::vector<PendingDeletion> m_pendingDeletions;//延迟销毁队列
		std::mutex m_deletionMutex;
	};
}