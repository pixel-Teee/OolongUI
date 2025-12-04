#pragma once

#include <vector>
#include <memory>

#include <Renderer/Renderer.h>

struct SDL_Window;

namespace Oolong
{
	class Application
	{
	public:
		Application();

		~Application();

		void createWindow();//create window

		SDL_Window* getDefaultWindow() const;

		static std::shared_ptr<Application> getApplication();

		std::shared_ptr<Renderer> getRenderer();

		void tick(float deltaTime);

		void shutDown();
	private:
		//window array
		std::vector<SDL_Window*> m_platformWindows;

		std::shared_ptr<Renderer> m_renderer;
	};
}