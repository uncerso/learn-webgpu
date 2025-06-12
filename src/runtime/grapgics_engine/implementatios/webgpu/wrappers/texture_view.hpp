#pragma once

#include "surface.hpp"
#include "texture.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.h>
#include <optional>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(TextureView, WGPUTextureView, view, wgpuTextureViewRelease);

class TextureView final : public TextureViewResourceHolder {
    explicit TextureView(WGPUTextureView view) {
        _view = view;
    }

public:
    static std::optional<TextureView> tryCreate(Surface const& surface);
    static TextureView create2DViewDepth(Texture const& texture);
};

} // namespace runtime::graphics_engine::webgpu
