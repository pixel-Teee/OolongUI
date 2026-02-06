#include "GPUResourceUtil.h"

#include <SDL3/SDL.h>
#include <Application/Application.h>

namespace Oolong
{
	std::shared_ptr<ResourceWrapper<SDL_GPUBuffer*>> createVertexBuffer(std::vector<Vertex> vertexData)
	{
		std::shared_ptr<Oolong::Application> application = Oolong::Application::getApplication();
		std::shared_ptr<Oolong::ResourceManager> resourceManager = application->getRenderer()->getResourceManager();
		// 创建顶点缓冲区
		SDL_GPUBufferCreateInfo bufferInfo{};
		bufferInfo.size = vertexData.size() * sizeof(Vertex);
		bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
		SDL_GPUBuffer* vertexBuffer = SDL_CreateGPUBuffer(application->getRenderer()->getGpuDevice(), &bufferInfo);

		// 创建一个传输缓冲区以上传到顶点缓冲区
		SDL_GPUTransferBufferCreateInfo transferInfo{};
		transferInfo.size = vertexData.size() * sizeof(Vertex);
		transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
		SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(application->getRenderer()->getGpuDevice(), &transferInfo);

		// 填充传输缓冲区
		Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(application->getRenderer()->getGpuDevice(), transferBuffer, false);

		SDL_memcpy(data, (void*)vertexData.data(), vertexData.size() * sizeof(Vertex));

		SDL_UnmapGPUTransferBuffer(application->getRenderer()->getGpuDevice(), transferBuffer);

		// 开始复制过程
		SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(application->getRenderer()->getGpuDevice());
		SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);

		// 数据所在位置
		SDL_GPUTransferBufferLocation location{};
		location.transfer_buffer = transferBuffer;
		location.offset = 0;

		// 数据上传位置
		SDL_GPUBufferRegion region{};
		region.buffer = vertexBuffer;
		region.size = vertexData.size() * sizeof(Vertex);
		region.offset = 0;

		// 上传数据
		SDL_UploadToGPUBuffer(copyPass, &location, &region, true);

		// 结束复制过程
		SDL_EndGPUCopyPass(copyPass);
		SDL_SubmitGPUCommandBuffer(commandBuffer);

		//auto transferBufferPtr = resourceManager->registerResource(
		//	transferBuffer
		//);
		
		return resourceManager->registerResource(vertexBuffer);
	}
}
