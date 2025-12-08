#pragma once

#include <memory>

#include <GPUResource/ResourceManager.h>

struct SDL_GPUDevice;

namespace Oolong
{
	class Demo;
	class Renderer
	{
	public:
		Renderer();

		~Renderer();

		void tick(float deltaTime);

		void createDemo();

		void renderUI();//render ui

		void shutDown();

		SDL_GPUDevice* getGpuDevice() const;

		std::shared_ptr<ResourceManager> getResourceManager();
	private:
		SDL_GPUDevice* m_gpuDevice;

		//gpu device
		std::shared_ptr<ResourceManager> m_gpuResourceManager;

		std::shared_ptr<Demo> m_testDemo;
	};
}