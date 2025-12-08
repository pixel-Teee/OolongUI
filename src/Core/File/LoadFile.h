#pragma once

#include <string>

namespace Oolong
{
	bool isDirectoyExist(const std::string& filePath);

	void* OolongLoadFile(const std::string& filePath, size_t* fileSize);
}