#include "render_pipeline.hpp"

#include <runtime/common/assert.hpp>

namespace runtime::graphics_engine::webgpu {

namespace {

WGPUFrontFace toWGPU(FrontFace frontFace) {
	switch (frontFace) {
        case FrontFace::CCW: return WGPUFrontFace_CCW;
        case FrontFace::CW:  return WGPUFrontFace_CW;
	}
}

WGPUPrimitiveTopology toWGPU(PrimitiveTopology topology) {
	switch (topology) {
        case PrimitiveTopology::PointList:     return  WGPUPrimitiveTopology_PointList;
        case PrimitiveTopology::LineList:      return  WGPUPrimitiveTopology_LineList;
        case PrimitiveTopology::LineStrip:     return  WGPUPrimitiveTopology_LineStrip;
        case PrimitiveTopology::TriangleList:  return  WGPUPrimitiveTopology_TriangleList;
        case PrimitiveTopology::TriangleStrip: return  WGPUPrimitiveTopology_TriangleStrip;
	}
}

WGPUCullMode toWGPU(CullMode cullMode) {
	switch (cullMode) {
        case CullMode::None:  return WGPUCullMode_None;
        case CullMode::Front: return WGPUCullMode_Front;
        case CullMode::Back:  return WGPUCullMode_Back;
	}
}

} // namespace

RenderPipeline::RenderPipeline(PipelineConfig config, ShaderModule const& shaderModule, Device const& device, Surface const& surface) {
	// Create the render pipeline
	WGPURenderPipelineDescriptor pipelineDesc{};
	pipelineDesc.nextInChain = nullptr;

	// We do not use any vertex buffer for this first simplistic example
	pipelineDesc.vertex.bufferCount = 0;
	pipelineDesc.vertex.buffers = nullptr;

	// NB: We define the 'shaderModule' in the second part of this chapter.
	// Here we tell that the programmable vertex shader stage is described
	// by the function called 'vs_main' in that module.
	pipelineDesc.vertex.module = shaderModule.module();
	pipelineDesc.vertex.entryPoint = "vs_main";
	pipelineDesc.vertex.constantCount = 0;
	pipelineDesc.vertex.constants = nullptr;

	pipelineDesc.primitive.topology = toWGPU(config.topology);

	// We'll see later how to specify the order in which vertices should be
	// connected. When not specified, vertices are considered sequentially.
	pipelineDesc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;

	pipelineDesc.primitive.frontFace = toWGPU(config.frontFace);
	pipelineDesc.primitive.cullMode = toWGPU(config.cullMode);

	WGPUFragmentState fragmentState{};
	fragmentState.module = shaderModule.module();
	fragmentState.entryPoint = "fs_main";
	fragmentState.constantCount = 0;
	fragmentState.constants = nullptr;

	WGPUBlendState blendState{};
	blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
	blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
	blendState.color.operation = WGPUBlendOperation_Add;
	blendState.alpha.srcFactor = WGPUBlendFactor_Zero;
	blendState.alpha.dstFactor = WGPUBlendFactor_One;
	blendState.alpha.operation = WGPUBlendOperation_Add;

	WGPUColorTargetState colorTarget{};
	colorTarget.format = surface.format();
	colorTarget.blend = &blendState;
	colorTarget.writeMask = WGPUColorWriteMask_All; // We could write to only some of the color channels.

	// We have only one target because our render pass has only one output color
	// attachment.
	fragmentState.targetCount = 1;
	fragmentState.targets = &colorTarget;
	pipelineDesc.fragment = &fragmentState;

	// We do not use stencil/depth testing for now
	pipelineDesc.depthStencil = nullptr;

	// Samples per pixel
	pipelineDesc.multisample.count = 1;

	// Default value for the mask, meaning "all bits on"
	pipelineDesc.multisample.mask = ~0u;

	// Default value as well (irrelevant for count = 1 anyways)
	pipelineDesc.multisample.alphaToCoverageEnabled = false;

	pipelineDesc.layout = nullptr;

	_pipeline = wgpuDeviceCreateRenderPipeline(device.device(), &pipelineDesc);
}

} // runtime::graphics_engine::webgpu
