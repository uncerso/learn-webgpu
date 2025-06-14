#pragma once

#include "shader_module.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "resource_holder.hpp"
#include "pipeline_layout.hpp"

#include <runtime/grapgics_engine/attribute.hpp>
#include <runtime/grapgics_engine/layout.hpp>
#include <runtime/grapgics_engine/pipeline_config.hpp>
#include <runtime/grapgics_engine/resources.hpp>
#include <webgpu/webgpu.h>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(RenderPipeline, WGPURenderPipeline, pipeline, wgpuRenderPipelineRelease);

class RenderPipeline final : public RenderPipelineResourceHolder, public IRenderPipeline {
public:
    RenderPipeline(
        PipelineConfig config,
        ShaderModule const& shaderModule,
        Device const& device,
        Surface const& surface,
        std::initializer_list<std::initializer_list<Attribute const>> vertexAttributes,
        std::initializer_list<Layout const> layouts);

    PipelineLayout const layout;

private:
WGPUDepthStencilState setupDepthStencil(PipelineConfig const& config);
};

} // namespace runtime::graphics_engine::webgpu
