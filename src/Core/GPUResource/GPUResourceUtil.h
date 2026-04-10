#pragma once

#include <vector>
#include <memory>
#include <Math/myMath.h>
#include <GPUResource/ResourceWrapper.h>

namespace Oolong
{
	//vertex layout
	struct Vertex
	{
		//vec3 position
		float3 position;
		//vec4 color
		float4 color;
	};

	struct UniformBuffer
	{
		float time;
		//你可以在这里添加其他属性
	};

	//create vertex gpu buffer
	ResourceWrapperPtr<SDL_GPUBuffer*> createVertexBuffer(std::vector<Vertex> vertexData);
}