#pragma once

#include "surface.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.hpp>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(TextureView, WGPUTextureView, view, wgpuTextureViewRelease);

class TextureView final : public TextureViewResourceHolder {
    TextureView(WGPUTextureView view) {
        _view = view;
    }

public:
    static std::optional<TextureView> tryCreate(Surface const& surface);
};

} // runtime::graphics_engine::webgpu
