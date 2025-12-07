#include "LoadFile.h"

#include <SDL3/SDL.h>

namespace Oolong
{

	bool isDirectoyExist(const std::string& filePath)
	{
		if (SDL_GetPathInfo(filePath.c_str(), nullptr))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void* OolongLoadFile(const std::string& filePath, size_t* fileSize)
	{
		if (isDirectoyExist(filePath))
		{
			void* buffer = SDL_LoadFile(filePath.c_str(), fileSize);
			return buffer;
		}
		else
		{
			std::string relativeFilePath = u8"../../";
			relativeFilePath += filePath;
			void* buffer = SDL_LoadFile(relativeFilePath.c_str(), fileSize);
			return buffer;
		}
	}
}