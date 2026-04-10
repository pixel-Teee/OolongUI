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
		ResourceWrapperPtr<SDL_GPUGraphicsPipeline*> m_trianglePipeline;

		ResourceWrapperPtr<SDL_GPUBuffer*> m_vertexBuffer;

		UniformBuffer m_uniformBuffer;
		//------triangle pipeline------
	};
}