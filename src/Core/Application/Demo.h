#pragma once

#include <memory>

#include <GPUResource/ResourceWrapper.h>
#include <GPUResource/GPUResourceUtil.h>

namespace Oolong
{
	//for test
	class Demo
	{
	public:
		Demo();

		~Demo();

		void tick();
	private:
		//------triangle pipeline------
		std::shared_ptr<ResourceWrapper<SDL_GPUGraphicsPipeline*>> m_trianglePipeline;

		std::shared_ptr<ResourceWrapper<SDL_GPUBuffer*>> m_vertexBuffer;

		UniformBuffer m_uniformBuffer;
		//------triangle pipeline------
	};
}