//SDL_MAIN_USE_CALLBACKS告知SDL_main.h去使用SDL回调作为入口
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#include <SDL3_shadercross/SDL_shadercross.h>

#include <string>

SDL_Window* window;
SDL_GPUDevice* device;
SDL_GPUBuffer* vertexBuffer;//顶点缓冲区
SDL_GPUTransferBuffer* transferBuffer;//复制缓冲区
SDL_GPUGraphicsPipeline* graphicsPipeline;//图形渲染管线对象

//vertex layout
struct Vertex
{
	float x, y, z;//vec3 position
	float r, g, b, a;//vec4 color
};

//vertex list
static Vertex vertices[] = {
	{ 0.0f , 0.5f , 0.0f , 1.0f , 0.0f , 0.0f , 1.0f }, //top vertex
	{ -0.5f , -0.5f , 0.0f , 1.0f , 1.0f , 0.0f , 1.0f }, //left bottom vertex
	{ 0.5f , -0.5f , 0.0f , 1.0f , 0.0f , 1.0f , 1.0f } //right bottom vertex
};

//SDL是[-1, 1]NDC坐标

struct  UniformBuffer
{
	float time;
	// 你可以在这里添加其他属性
};

static UniformBuffer timeUniform{};

//convert utf16 to utf8
char* covertUtf16ToUtf8(const wchar_t* wStr)
{
	const char* srcEncoding = "UTF-16LE"; // 指定源编码
	const char* dstEncoding = "UTF-8";    // 指定目标编码
	const wchar_t* wstr = wStr; // 你的宽字符串
	size_t wstr_len = SDL_wcslen(wStr); // 宽字符串的字符数（非字节数）
	// 进行转换。注意：SDL_iconv_string需要传入以null结尾的字符串。
	char* utf8Str = SDL_iconv_string(dstEncoding, srcEncoding,
		(const char*)wstr, (wstr_len + 1) * sizeof(wchar_t));
	return utf8Str;
}

bool isDirectoyExist(const wchar_t* filePath)
{
	const char* utf8Str = covertUtf16ToUtf8(filePath);
	if (SDL_GetPathInfo(utf8Str, nullptr))
	{
		SDL_free((void*)utf8Str);
		return true;
	}
	else
	{
		SDL_free((void*)utf8Str);
		return false;
	}
}

void* OolongLoadFile(const wchar_t* filePath, size_t* fileSize)
{
	if (isDirectoyExist(filePath))
	{
		const char* utf8Str = covertUtf16ToUtf8(filePath);
		void* buffer = SDL_LoadFile(utf8Str, fileSize);
		SDL_free((void*)utf8Str);
		return buffer;
	}
	else
	{
		std::wstring relativeFilePath = L"../../";
		relativeFilePath += filePath;
		const char* utf8Str = covertUtf16ToUtf8(relativeFilePath.c_str());
		void* buffer = SDL_LoadFile(utf8Str, fileSize);
		SDL_free((void*)utf8Str);
		return buffer;
	}
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	//create a window
	window = SDL_CreateWindow("Hello, Triangle!", 960, 540, SDL_WINDOW_RESIZABLE);

	//访问GPU设备
	//1.着色器格式
	//2.gpu 调试层
	//3.驱动名称
	device = SDL_CreateGPUDevice(SDL_ShaderCross_GetSPIRVShaderFormats(), false, NULL);
	if (device == nullptr)
	{
		SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_GPU, "create gpu device error");
	}
	else
	{
		SDL_LogInfo(SDL_LogCategory::SDL_LOG_CATEGORY_GPU, "create gpu device successful");
		//关联设备和窗口
		SDL_ClaimWindowForGPUDevice(device, window);
	}

	//加载着色器代码
	size_t vertexCodeSize;
	void* vertexCode = OolongLoadFile(L"shaders/vertex.spv", &vertexCodeSize);

	//创建顶点着色器对象
	//SDL_GPUShaderCreateInfo vertexInfo{};
	//vertexInfo.code = (Uint8*)vertexCode;
	//vertexInfo.code_size = vertexCodeSize;
	//vertexInfo.entrypoint = "main";
	//vertexInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
	//vertexInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
	//vertexInfo.num_samplers = 0;
	//vertexInfo.num_storage_buffers = 0;
	//vertexInfo.num_storage_textures = 0;
	//vertexInfo.num_uniform_buffers = 0;
	//
	//SDL_GPUShader* vertexShader = SDL_CreateGPUShader(device, &vertexInfo);
	//
	////释放文件
	//SDL_free(vertexCode);

	//加载片段着色器代码
	size_t fragmentCodeSize;
	void* fragmentCode = OolongLoadFile(L"shaders/fragment.spv", &fragmentCodeSize);

	//创建片段着色器
	//SDL_GPUShaderCreateInfo fragmentInfo{};
	//fragmentInfo.code = (Uint8*)fragmentCode;
	//fragmentInfo.code_size = fragmentCodeSize;
	//fragmentInfo.entrypoint = "main";
	//fragmentInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
	//fragmentInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	//fragmentInfo.num_samplers = 0;
	//fragmentInfo.num_storage_buffers = 0;
	//fragmentInfo.num_storage_textures = 0;
	//fragmentInfo.num_uniform_buffers = 1;
	//
	//SDL_GPUShader* fragmentShader = SDL_CreateGPUShader(device, &fragmentInfo);
	//
	////释放文件
	//SDL_free(fragmentCode);

	
	SDL_ShaderCross_SPIRV_Info vertexInfo{};
	vertexInfo.bytecode = (Uint8*)vertexCode;
	vertexInfo.bytecode_size = vertexCodeSize;
	vertexInfo.entrypoint = "main";
	vertexInfo.shader_stage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX;

	SDL_ShaderCross_GraphicsShaderMetadata* vertexMetadata = SDL_ShaderCross_ReflectGraphicsSPIRV((Uint8*)vertexCode, vertexCodeSize, 0);

	SDL_GPUShader* vertexShader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(device, &vertexInfo, &vertexMetadata->resource_info, 0);

	SDL_free(vertexMetadata);

	SDL_ShaderCross_SPIRV_Info fragmentInfo{};
	fragmentInfo.bytecode = (Uint8*)fragmentCode;
	fragmentInfo.bytecode_size = fragmentCodeSize;
	fragmentInfo.entrypoint = "main";
	fragmentInfo.shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;

	SDL_ShaderCross_GraphicsShaderMetadata* framgentMetadata = SDL_ShaderCross_ReflectGraphicsSPIRV((Uint8*)fragmentCode, fragmentCodeSize, 0);
	SDL_GPUShader* fragmentShader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(device, &fragmentInfo, &framgentMetadata->resource_info, 0);
	SDL_free(framgentMetadata);
	
	SDL_free(vertexCode);
	SDL_free(fragmentCode);

	// 创建图形管线
	SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.vertex_shader = vertexShader;
	pipelineInfo.fragment_shader = fragmentShader;
	pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

	// 描述顶点缓冲区
	SDL_GPUVertexBufferDescription vertexBufferDesctiptions[1];
	vertexBufferDesctiptions[0].slot = 0;
	vertexBufferDesctiptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
	vertexBufferDesctiptions[0].instance_step_rate = 0;
	vertexBufferDesctiptions[0].pitch = sizeof(Vertex);

	pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
	pipelineInfo.vertex_input_state.vertex_buffer_descriptions = vertexBufferDesctiptions;

	// 描述顶点属性
	SDL_GPUVertexAttribute vertexAttributes[2];

	// a_position
	vertexAttributes[0].buffer_slot = 0;
	vertexAttributes[0].location = 0;
	vertexAttributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
	vertexAttributes[0].offset = 0;

	// a_color
	vertexAttributes[1].buffer_slot = 0;
	vertexAttributes[1].location = 1;
	vertexAttributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
	vertexAttributes[1].offset = sizeof(float) * 3;

	pipelineInfo.vertex_input_state.num_vertex_attributes = 2;
	pipelineInfo.vertex_input_state.vertex_attributes = vertexAttributes;

	// 描述颜色目标
	SDL_GPUColorTargetDescription colorTargetDescriptions[1];
	colorTargetDescriptions[0] = {};
	colorTargetDescriptions[0].blend_state.enable_blend = true;
	colorTargetDescriptions[0].blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
	colorTargetDescriptions[0].blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
	colorTargetDescriptions[0].blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	colorTargetDescriptions[0].blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	colorTargetDescriptions[0].blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	colorTargetDescriptions[0].blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	colorTargetDescriptions[0].format = SDL_GetGPUSwapchainTextureFormat(device, window);

	pipelineInfo.target_info.num_color_targets = 1;
	pipelineInfo.target_info.color_target_descriptions = colorTargetDescriptions;

	// 创建管道
	graphicsPipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);

	// 创建管道后无需存储着色器
	SDL_ReleaseGPUShader(device, vertexShader);
	SDL_ReleaseGPUShader(device, fragmentShader);

	// 创建顶点缓冲区
	SDL_GPUBufferCreateInfo bufferInfo{};
	bufferInfo.size = sizeof(vertices);
	bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
	vertexBuffer = SDL_CreateGPUBuffer(device, &bufferInfo);

	// 创建一个传输缓冲区以上传到顶点缓冲区
	SDL_GPUTransferBufferCreateInfo transferInfo{};
	transferInfo.size = sizeof(vertices);
	transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

	// 填充传输缓冲区
	Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(device, transferBuffer, false);

	SDL_memcpy(data, (void*)vertices, sizeof(vertices));

	// data[0] = vertices[0]; 
	// data[1] = vertices[1]; 
	// data[2] = vertices[2]; 

	SDL_UnmapGPUTransferBuffer(device, transferBuffer);

	// 开始复制过程
	SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);

	// 数据所在位置
	SDL_GPUTransferBufferLocation location{};
	location.transfer_buffer = transferBuffer;
	location.offset = 0;

	// 数据上传位置
	SDL_GPUBufferRegion region{};
	region.buffer = vertexBuffer;
	region.size = sizeof(vertices);
	region.offset = 0;

	// 上传数据
	SDL_UploadToGPUBuffer(copyPass, &location, &region, true);

	// 结束复制过程
	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(commandBuffer);

	return SDL_APP_CONTINUE;
}

//update回调
SDL_AppResult SDL_AppIterate(void* appstate)
{
	// 获取命令缓冲区
	SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(device);

	// 获取交换链纹理
	SDL_GPUTexture* swapchainTexture;
	Uint32 width, height;
	SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, window, &swapchainTexture, &width, &height);

	// 如果交换链纹理不可用，则提前结束帧
	if (swapchainTexture == NULL)
	{
		// 必须始终提交命令缓冲区
		SDL_SubmitGPUCommandBuffer(commandBuffer);
		return SDL_APP_CONTINUE;
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
	SDL_BindGPUGraphicsPipeline(renderPass, graphicsPipeline);

	// 绑定顶点缓冲区
	SDL_GPUBufferBinding bufferBindings[1];
	bufferBindings[0].buffer = vertexBuffer;
	bufferBindings[0].offset = 0;

	SDL_BindGPUVertexBuffers(renderPass, 0, bufferBindings, 1);

	timeUniform.time = SDL_GetTicksNS() / 1e9f;//自应用程序启动以来的时间（以秒为单位）
	SDL_PushGPUFragmentUniformData(commandBuffer, 0, &timeUniform, sizeof(UniformBuffer));

	// 发出绘制调用
	SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

	// 结束渲染通道
	SDL_EndGPURenderPass(renderPass);

	// 提交命令缓冲区
	SDL_SubmitGPUCommandBuffer(commandBuffer);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	// close the window on request
	if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
	{
		return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	// 释放缓冲区
	SDL_ReleaseGPUBuffer(device, vertexBuffer);
	SDL_ReleaseGPUTransferBuffer(device, transferBuffer);

	// 释放管道
	SDL_ReleaseGPUGraphicsPipeline(device, graphicsPipeline);

	//销毁GPU设备
	SDL_DestroyGPUDevice(device);

	// destroy the window
	SDL_DestroyWindow(window);
}

