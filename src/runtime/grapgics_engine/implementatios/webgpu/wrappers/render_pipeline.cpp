#include "render_pipeline.hpp"

#include <runtime/common/assert.hpp>
#include <span>

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

WGPUVertexFormat toWGPU(Attribute::Type type) {
    switch (type) {
    case Attribute::Type::Float32:   return WGPUVertexFormat_Float32;
    case Attribute::Type::Float32x2: return WGPUVertexFormat_Float32x2;
    case Attribute::Type::Float32x3: return WGPUVertexFormat_Float32x3;
    case Attribute::Type::Float32x4: return WGPUVertexFormat_Float32x4;
    case Attribute::Type::UInt32:    return WGPUVertexFormat_Uint32;
    case Attribute::Type::UInt32x2:  return WGPUVertexFormat_Uint32x2;
    case Attribute::Type::UInt32x3:  return WGPUVertexFormat_Uint32x3;
    case Attribute::Type::UInt32x4:  return WGPUVertexFormat_Uint32x4;
    }
}

uint32_t toElementCount(Attribute::Type type) {
    switch (type) {
    case Attribute::Type::Float32:   return 1;
    case Attribute::Type::Float32x2: return 2;
    case Attribute::Type::Float32x3: return 3;
    case Attribute::Type::Float32x4: return 4;
    case Attribute::Type::UInt32:    return 1;
    case Attribute::Type::UInt32x2:  return 2;
    case Attribute::Type::UInt32x3:  return 3;
    case Attribute::Type::UInt32x4:  return 4;
    }
}

uint32_t singleElementTypeSize(Attribute::Type type) {
    switch (type) {
    case Attribute::Type::Float32:   [[fallthrough]];
    case Attribute::Type::Float32x2: [[fallthrough]];
    case Attribute::Type::Float32x3: [[fallthrough]];
    case Attribute::Type::Float32x4: [[fallthrough]];
    case Attribute::Type::UInt32:    [[fallthrough]];
    case Attribute::Type::UInt32x2:  [[fallthrough]];;
    case Attribute::Type::UInt32x3:  [[fallthrough]];
    case Attribute::Type::UInt32x4:  return 4;
    }
}

// returns stride
size_t fillVertexAttribute(std::span<const Attribute> vertexAttributes, WGPUVertexAttribute* attributes, uint32_t startLocation = 0) {
    uint32_t offset = 0;
    for (uint32_t i = 0; i < vertexAttributes.size(); ++i) {
        auto& type = vertexAttributes[i].type;
        attributes[i].shaderLocation = startLocation + i;
        attributes[i].format = toWGPU(type);
        attributes[i].offset = offset;
        offset += singleElementTypeSize(type) * toElementCount(type);
    }
    return offset;
}

void fillVertexLayouts(std::initializer_list<std::initializer_list<Attribute const>> vertexAttributes, WGPUVertexBufferLayout* layouts, WGPUVertexAttribute* attributes) {
    uint32_t startLocation = 0;
    for (auto const& vertexAttribute : vertexAttributes) {
        layouts->attributeCount = vertexAttribute.size();
        layouts->attributes = attributes;
        layouts->stepMode = WGPUVertexStepMode_Vertex;
        layouts->arrayStride = fillVertexAttribute(vertexAttribute, attributes, startLocation);

        startLocation += layouts->attributeCount;
        attributes += layouts->attributeCount;
        ++layouts;
    }
}

size_t computeFlattenSize(std::initializer_list<std::initializer_list<Attribute const>> spans) {
    size_t size = 0;
    for (auto const& span : spans)
        size += span.size();
    return size;
}

} // namespace

RenderPipeline::RenderPipeline(
    PipelineConfig config,
    ShaderModule const& shaderModule,
    Device const& device,
    Surface const& surface,
    std::initializer_list<std::initializer_list<Attribute const>> vertexAttributes)
{
    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.nextInChain = nullptr;

    auto const flattenSize = computeFlattenSize(vertexAttributes);
    WGPUVertexAttribute attributes[flattenSize];
    WGPUVertexBufferLayout vertexBufferLayouts[vertexAttributes.size()];
    fillVertexLayouts(vertexAttributes, vertexBufferLayouts, attributes);

    pipelineDesc.vertex.bufferCount = vertexAttributes.size();
    pipelineDesc.vertex.buffers = (vertexAttributes.size() == 0 ? nullptr : vertexBufferLayouts);


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

} // namespace runtime::graphics_engine::webgpu
