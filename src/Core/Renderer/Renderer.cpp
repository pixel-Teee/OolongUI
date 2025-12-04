#include "Renderer.h"

#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>//SDL_ShaderCross_GetSPIRVShaderFormats

namespace Oolong
{

	Renderer::Renderer()
	{
		m_gpuDevice = SDL_CreateGPUDevice(SDL_ShaderCross_GetSPIRVShaderFormats(), false, NULL);
		//const char* errorInfo = SDL_GetError();
		//SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_ERROR, errorInfo);
		m_gpuResourceManager = std::make_shared<ResourceManager>(m_gpuDevice);
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::tick(float deltaTime)
	{

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

}