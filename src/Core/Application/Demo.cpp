#include "Demo.h"

#include <File/LoadFile.h>
#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include <Application/Application.h>
#include <GPUResource/ResourceManager.h>
#include <GPUResource/GPUResourceUtil.h>

namespace Oolong
{
	Demo::Demo()
	{
		//vertex list
		std::vector<Vertex> vertices = {
			{ 0.0f , 0.5f , 0.0f , 1.0f , 0.0f , 0.0f , 1.0f }, //top vertex
			{ -0.5f , -0.5f , 0.0f , 1.0f , 1.0f , 0.0f , 1.0f }, //left bottom vertex
			{ 0.5f , -0.5f , 0.0f , 1.0f , 0.0f , 1.0f , 1.0f } //right bottom vertex
		};

		std::shared_ptr<Oolong::Application> application = Oolong::Application::getApplication();
		std::shared_ptr<Oolong::ResourceManager> resourceManager = application->getRenderer()->getResourceManager();

		//加载着色器代码
		size_t vertexCodeSize;
		void* vertexCode = OolongLoadFile(u8"shaders/vertex.spv", &vertexCodeSize);

		//加载片段着色器代码
		size_t fragmentCodeSize;
		void* fragmentCode = OolongLoadFile(u8"shaders/fragment.spv", &fragmentCodeSize);

		SDL_ShaderCross_SPIRV_Info vertexInfo{};
		vertexInfo.bytecode = (Uint8*)vertexCode;
		vertexInfo.bytecode_size = vertexCodeSize;
		vertexInfo.entrypoint = "main";
		vertexInfo.shader_stage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX;

		SDL_ShaderCross_GraphicsShaderMetadata* vertexMetadata = SDL_ShaderCross_ReflectGraphicsSPIRV((Uint8*)vertexCode, vertexCodeSize, 0);
		SDL_GPUShader* vertexShader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(application->getRenderer()->getGpuDevice(), &vertexInfo, &vertexMetadata->resource_info, 0);
		SDL_free(vertexMetadata);

		SDL_ShaderCross_SPIRV_Info fragmentInfo{};
		fragmentInfo.bytecode = (Uint8*)fragmentCode;
		fragmentInfo.bytecode_size = fragmentCodeSize;
		fragmentInfo.entrypoint = "main";
		fragmentInfo.shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;

		SDL_ShaderCross_GraphicsShaderMetadata* framgentMetadata = SDL_ShaderCross_ReflectGraphicsSPIRV((Uint8*)fragmentCode, fragmentCodeSize, 0);
		SDL_GPUShader* fragmentShader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(application->getRenderer()->getGpuDevice(), &fragmentInfo, &framgentMetadata->resource_info, 0);
		SDL_free(framgentMetadata);

		SDL_free(vertexCode);
		SDL_free(fragmentCode);

		//创建图形管线
		SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.vertex_shader = vertexShader;
		pipelineInfo.fragment_shader = fragmentShader;
		pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

		//描述顶点缓冲区
		SDL_GPUVertexBufferDescription vertexBufferDesctiptions[1];
		vertexBufferDesctiptions[0].slot = 0;
		vertexBufferDesctiptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
		vertexBufferDesctiptions[0].instance_step_rate = 0;
		vertexBufferDesctiptions[0].pitch = sizeof(Vertex);

		pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
		pipelineInfo.vertex_input_state.vertex_buffer_descriptions = vertexBufferDesctiptions;

		// 描述顶点属性
		SDL_GPUVertexAttribute vertexAttributes[2];

		//a_position
		vertexAttributes[0].buffer_slot = 0;
		vertexAttributes[0].location = 0;
		vertexAttributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		vertexAttributes[0].offset = 0;

		//a_color
		vertexAttributes[1].buffer_slot = 0;
		vertexAttributes[1].location = 1;
		vertexAttributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
		vertexAttributes[1].offset = sizeof(float) * 3;

		pipelineInfo.vertex_input_state.num_vertex_attributes = 2;
		pipelineInfo.vertex_input_state.vertex_attributes = vertexAttributes;

		//描述颜色目标
		SDL_GPUColorTargetDescription colorTargetDescriptions[1];
		colorTargetDescriptions[0] = {};
		colorTargetDescriptions[0].blend_state.enable_blend = true;
		colorTargetDescriptions[0].blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
		colorTargetDescriptions[0].blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
		colorTargetDescriptions[0].blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
		colorTargetDescriptions[0].blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
		colorTargetDescriptions[0].blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
		colorTargetDescriptions[0].blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
		colorTargetDescriptions[0].format = SDL_GetGPUSwapchainTextureFormat(application->getRenderer()->getGpuDevice(), application->getDefaultWindow());

		pipelineInfo.target_info.num_color_targets = 1;
		pipelineInfo.target_info.color_target_descriptions = colorTargetDescriptions;

		//创建管道
		auto graphicsPipeline = SDL_CreateGPUGraphicsPipeline(application->getRenderer()->getGpuDevice(), &pipelineInfo);
		m_trianglePipeline = resourceManager->registerResource(
			graphicsPipeline
		);

		SDL_ReleaseGPUShader(application->getRenderer()->getGpuDevice(), vertexShader);
		SDL_ReleaseGPUShader(application->getRenderer()->getGpuDevice(), fragmentShader);

		m_vertexBuffer = createVertexBuffer(vertices);
	}

	Demo::~Demo()
	{

	}

	void Demo::tick()
	{
		std::shared_ptr<Oolong::Application> application = Oolong::Application::getApplication();
		
		// 获取命令缓冲区
		SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(application->getRenderer()->getGpuDevice());

		// 获取交换链纹理
		SDL_GPUTexture* swapchainTexture;
		Uint32 width, height;
		SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, application->getDefaultWindow(), &swapchainTexture, &width, &height);

		// 如果交换链纹理不可用，则提前结束帧
		if (swapchainTexture == NULL)
		{
			// 必须始终提交命令缓冲区
			SDL_SubmitGPUCommandBuffer(commandBuffer);
			//return SDL_APP_CONTINUE;
		}

		// 创建颜色目标
		SDL_GPUColorTargetInfo colorTargetInfo{};
		colorTargetInfo.clear_color = { 240 / 255.0f , 240 / 255.0f , 240 / 255.0f , 255 / 255.0f };
		colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;//丢弃先前的内容并清除颜色，SDL_GPU_LOADOP_LOAD以保留先前的内容
		colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;//将内容存储到纹理
		colorTargetInfo.texture = swapchainTexture;//操作的是swap chain texture

		// 开始渲染通道
		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, NULL);

		// 绑定渲染管线
		SDL_BindGPUGraphicsPipeline(renderPass, m_trianglePipeline->get());

		// 绑定顶点缓冲区
		SDL_GPUBufferBinding bufferBindings[1];
		bufferBindings[0].buffer = m_vertexBuffer->get();
		bufferBindings[0].offset = 0;

		SDL_BindGPUVertexBuffers(renderPass, 0, bufferBindings, 1);

		m_uniformBuffer.time = SDL_GetTicksNS() / 1e9f;//自应用程序启动以来的时间（以秒为单位）
		SDL_PushGPUFragmentUniformData(commandBuffer, 0, &m_uniformBuffer, sizeof(m_uniformBuffer));

		// 发出绘制调用
		SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

		// 结束渲染通道
		SDL_EndGPURenderPass(renderPass);

		// 提交命令缓冲区
		SDL_SubmitGPUCommandBuffer(commandBuffer);
	}

}