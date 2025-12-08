#pragma once

#include <vector>
#include <memory>

#include <GPUResource/ResourceWrapper.h>

namespace Oolong
{
	//vertex layout
	struct Vertex
	{
		float x, y, z;//vec3 position
		float r, g, b, a;//vec4 color
	};

	struct  UniformBuffer
	{
		float time;
		//你可以在这里添加其他属性
	};

	//create vertex gpu buffer
	std::shared_ptr<ResourceWrapper<SDL_GPUBuffer*>> createVertexBuffer(std::vector<Vertex> vertexData);
}