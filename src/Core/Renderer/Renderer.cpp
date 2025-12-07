#include "Renderer.h"

#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>//SDL_ShaderCross_GetSPIRVShaderFormats

#include <Application/Demo.h>

namespace Oolong
{

	Renderer::Renderer()
	{
		m_gpuDevice = SDL_CreateGPUDevice(SDL_ShaderCross_GetSPIRVShaderFormats(), false, NULL);

		m_gpuResourceManager = std::make_shared<ResourceManager>(m_gpuDevice);	
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::tick(float deltaTime)
	{
		m_testDemo->tick();

		m_gpuResourceManager->tryCleanup();
	}

	void Renderer::createDemo()
	{
		m_testDemo = std::make_shared<Demo>();
	}

	void Renderer::renderUI()
	{

	}

	void Renderer::shutDown()
	{
		//destroy gpu device
		m_gpuResourceManager->shutDown();

		SDL_DestroyGPUDevice(m_gpuDevice);
	}

	SDL_GPUDevice* Renderer::getGpuDevice() const
	{
		return m_gpuDevice;
	}

	std::shared_ptr<ResourceManager> Renderer::getResourceManager()
	{
		return m_gpuResourceManager;
	}

}