#include <SDL3/SDL_log.h>

int main()
{
	//entry point
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "entry point");

	while (1)
	{
		//nothing
		//SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "entry point");
	}
	return 0;
}

