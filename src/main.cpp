//SDL_MAIN_USE_CALLBACKS告知SDL_main.h去使用SDL回调作为入口
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>
#include <string>
#include <GPUResource/ResourceManager.h>
#include <Application/Application.h>

//SDL是[-1, 1]NDC坐标

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	std::shared_ptr<Oolong::Application> application = Oolong::Application::getApplication();
	application->createDemo();
	return SDL_APP_CONTINUE;
}

//update回调
SDL_AppResult SDL_AppIterate(void* appstate)
{
	std::shared_ptr<Oolong::Application> application = Oolong::Application::getApplication();
	application->tick();

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	// close the window on request
	if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
	{
		return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	std::shared_ptr<Oolong::Application> application = Oolong::Application::getApplication();
	std::shared_ptr<Oolong::ResourceManager> resourceManager = application->getRenderer()->getResourceManager();
	resourceManager->tryCleanup();

	application->shutDown();
}

