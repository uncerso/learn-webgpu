#pragma once

#include "surface.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.h>
#include <optional>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(TextureView, WGPUTextureView, view, wgpuTextureViewRelease);

class TextureView final : public TextureViewResourceHolder {
    TextureView(WGPUTextureView view) {
        _view = view;
    }

public:
    static std::optional<TextureView> tryCreate(Surface const& surface);
};

} // namespace runtime::graphics_engine::webgpu
