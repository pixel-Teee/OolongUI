#pragma once

#include <memory>

#include <GPUResource/ResourceManager.h>

struct SDL_GPUDevice;

namespace Oolong
{
	class Renderer
	{
	public:
		Renderer();

		~Renderer();

		void tick(float deltaTime);

		void renderUI();//render ui

		void shutDown();

		SDL_GPUDevice* getGpuDevice() const;
	private:
		SDL_GPUDevice* m_gpuDevice;

		//gpu device
		std::shared_ptr<ResourceManager> m_gpuResourceManager;
	};
}