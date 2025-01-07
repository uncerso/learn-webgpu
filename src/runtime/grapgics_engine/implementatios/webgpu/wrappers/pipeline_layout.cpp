#include "pipeline_layout.hpp"

#include <runtime/common/assert.hpp>

namespace runtime::graphics_engine::webgpu {

namespace {

WGPUBindGroupLayoutEntry defaultBindGroupLayoutEntry() {
    WGPUBindGroupLayoutEntry bindingLayout{};
    bindingLayout.nextInChain = nullptr;

    bindingLayout.buffer.nextInChain = nullptr;
    bindingLayout.buffer.type = WGPUBufferBindingType_Undefined;
    bindingLayout.buffer.hasDynamicOffset = false;
    bindingLayout.buffer.minBindingSize = 0;

    bindingLayout.sampler.nextInChain = nullptr;
    bindingLayout.sampler.type = WGPUSamplerBindingType_Undefined;

    bindingLayout.storageTexture.nextInChain = nullptr;
    bindingLayout.storageTexture.access = WGPUStorageTextureAccess_Undefined;
    bindingLayout.storageTexture.format = WGPUTextureFormat_Undefined;
    bindingLayout.storageTexture.viewDimension = WGPUTextureViewDimension_Undefined;

    bindingLayout.texture.nextInChain = nullptr;
    bindingLayout.texture.multisampled = false;
    bindingLayout.texture.sampleType = WGPUTextureSampleType_Undefined;
    bindingLayout.texture.viewDimension = WGPUTextureViewDimension_Undefined;

    return bindingLayout;
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
        bindingLayout.binding = layout.bindingPoint;
        bindingLayout.visibility = toWGPU(layout.access);

        WGPUBindGroupLayoutDescriptor bindGroupLayoutDesc{};
        bindGroupLayoutDesc.nextInChain = nullptr;
        bindGroupLayoutDesc.label = nullptr;
        bindGroupLayoutDesc.entryCount = 1;
        bindGroupLayoutDesc.entries = &bindingLayout;
        _bindGroupLayouts.push_back(wgpuDeviceCreateBindGroupLayout(device.device(), &bindGroupLayoutDesc));
    }

    WGPUPipelineLayoutDescriptor layoutDesc{};
    layoutDesc.nextInChain = nullptr;
    layoutDesc.bindGroupLayoutCount = _bindGroupLayouts.size();
    layoutDesc.bindGroupLayouts = _bindGroupLayouts.data();
    _layout = wgpuDeviceCreatePipelineLayout(device.device(), &layoutDesc);
}

PipelineLayout::~PipelineLayout() {
    for (auto const& layout : _bindGroupLayouts)
        wgpuBindGroupLayoutRelease(layout);
    if (_layout) // should check nullptr?
        wgpuPipelineLayoutRelease(_layout);
}


} // namespace runtime::graphics_engine::webgpu
