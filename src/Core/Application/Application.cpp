#include "Application.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

namespace Oolong
{

	Application::Application()
	{
		SDL_Init(SDL_INIT_VIDEO);
		//create renderer
		m_renderer = std::make_shared<Renderer>();

		createWindow();
	}

	Application::~Application()
	{

	}

	void Application::createWindow()
	{
		SDL_Window* platformWindow = SDL_CreateWindow("Hello, Triangle!", 960, 540, SDL_WINDOW_RESIZABLE);
		if (platformWindow)
		{
			SDL_LogInfo(SDL_LogCategory::SDL_LOG_CATEGORY_APPLICATION, "create window successful");
		}
		else
		{
			SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_APPLICATION, "create window error");
		}

		m_platformWindows.push_back(platformWindow);

		//关联 window 和 gpu device
		SDL_GPUDevice* gpuDevice = getRenderer()->getGpuDevice();
		SDL_ClaimWindowForGPUDevice(gpuDevice, platformWindow);
	}

	SDL_Window* Application::getDefaultWindow() const
	{
		if(m_platformWindows.size() > 0)
			return m_platformWindows[0];
		return nullptr;
	}

	std::shared_ptr<Application> Application::getApplication()
	{
		static std::shared_ptr<Application> application = std::make_shared<Application>();
		return application;
	}

	std::shared_ptr<Renderer> Application::getRenderer()
	{
		return m_renderer;
	}

	void Application::tick(float deltaTime)
	{
		m_renderer->tick(deltaTime);
	}

	void Application::shutDown()
	{
		m_renderer->shutDown();

		for (int32_t i = 0; i < m_platformWindows.size(); ++i)
		{
			SDL_DestroyWindow(m_platformWindows[i]);
		}
	}

}