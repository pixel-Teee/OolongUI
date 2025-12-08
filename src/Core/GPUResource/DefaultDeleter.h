#pragma once

#include <SDL3/SDL.h>

namespace Oolong
{
	template<typename ResourceType>
	struct DefaultDeleter;

	template<>
	struct DefaultDeleter<SDL_GPUShader*>
	{
		void operator()(SDL_GPUDevice* gpuDevice, SDL_GPUShader* shader) const
		{
			SDL_ReleaseGPUShader(gpuDevice, shader);
		}
	};

	template<>
	struct DefaultDeleter<SDL_GPUGraphicsPipeline*>
	{
		void operator()(SDL_GPUDevice* gpuDevice, SDL_GPUGraphicsPipeline* pso) const
		{
			SDL_ReleaseGPUGraphicsPipeline(gpuDevice, pso);
		}
	};

	template<>
	struct DefaultDeleter<SDL_GPUTransferBuffer*>
	{
		void operator()(SDL_GPUDevice* gpuDevice, SDL_GPUTransferBuffer* buffer) const
		{
			SDL_ReleaseGPUTransferBuffer(gpuDevice, buffer);
		}
	};

	template<>
	struct DefaultDeleter<SDL_GPUBuffer*>
	{
		void operator()(SDL_GPUDevice* gpuDevice, SDL_GPUBuffer* buffer) const
		{
			SDL_ReleaseGPUBuffer(gpuDevice, buffer);
		}
	};
}