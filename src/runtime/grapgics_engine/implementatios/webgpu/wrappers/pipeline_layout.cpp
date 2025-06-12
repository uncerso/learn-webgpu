#include "pipeline_layout.hpp"

#include <runtime/common/assert.hpp>

namespace runtime::graphics_engine::webgpu {

namespace {

WGPUBindGroupLayoutEntry defaultBindGroupLayoutEntry() {
    return {
        .nextInChain = nullptr,

        .buffer {
            .nextInChain = nullptr,
            .type = WGPUBufferBindingType_Undefined,
            .hasDynamicOffset = false,
            .minBindingSize = 0,
        },

        .sampler {
            .nextInChain = nullptr,
            .type = WGPUSamplerBindingType_Undefined,
        },

        .texture {
            .nextInChain = nullptr,
            .sampleType = WGPUTextureSampleType_Undefined,
            .viewDimension = WGPUTextureViewDimension_Undefined,
            .multisampled = false,
        },

        .storageTexture {
            .nextInChain = nullptr,
            .access = WGPUStorageTextureAccess_Undefined,
            .format = WGPUTextureFormat_Undefined,
            .viewDimension = WGPUTextureViewDimension_Undefined,
        },
    };
}

WGPUShaderStageFlags toWGPU(Layout::Access access) {
    switch (access) {
        case Layout::Access::Vertex: return WGPUShaderStage_Vertex;
        case Layout::Access::Fragment: return WGPUShaderStage_Fragment;
        case Layout::Access::VertexAndFragment: return WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
    }
}

} // namespace

PipelineLayout::PipelineLayout(Device const& device, std::initializer_list<Layout const> layouts) {
    if (layouts.size() == 0)
        return;

    _bindGroupLayouts.reserve(layouts.size());
    for (auto const& layout : layouts) {
        auto bindingLayout = defaultBindGroupLayoutEntry();
        bindingLayout.buffer.type = WGPUBufferBindingType_Uniform;
        ASSERT(_bindGroupLayouts.size() == layout.bindingPoint); // TODO
        bindingLayout.binding = layout.bindingPoint;
        bindingLayout.visibility = toWGPU(layout.access);

        WGPUBindGroupLayoutDescriptor bindGroupLayoutDesc {
            .nextInChain = nullptr,
            .label = nullptr,
            .entryCount = 1,
            .entries = &bindingLayout,
        };
        _bindGroupLayouts.push_back(wgpuDeviceCreateBindGroupLayout(device.device(), &bindGroupLayoutDesc));
    }

    WGPUPipelineLayoutDescriptor layoutDesc {
        .nextInChain = nullptr,
        .label = nullptr,
        .bindGroupLayoutCount = _bindGroupLayouts.size(),
        .bindGroupLayouts = _bindGroupLayouts.data(),
    };
    _layout = wgpuDeviceCreatePipelineLayout(device.device(), &layoutDesc);
}

PipelineLayout::~PipelineLayout() {
    for (auto const& layout : _bindGroupLayouts)
        wgpuBindGroupLayoutRelease(layout);
    if (_layout) // should check nullptr?
        wgpuPipelineLayoutRelease(_layout);
}


} // namespace runtime::graphics_engine::webgpu
