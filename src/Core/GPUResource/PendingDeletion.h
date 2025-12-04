#pragma once

#include <functional>

namespace Oolong
{
	struct PendingDeletion
	{
		using DeleterFunc = std::function<void()>;

		DeleterFunc deleter;//执行最终销毁
		uint64_t frameIndex;//该资源被标记为待删除时的帧序号

		PendingDeletion(DeleterFunc del, uint64_t frame)
			: deleter(std::move(del)), frameIndex(frame) {}
	};
}