#pragma once

#include "device.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.h>

namespace runtime::graphics_engine::webgpu {

namespace internal {
void textureDestructor(WGPUTexture texture);
} // internal

RESOURCE_HOLDER(Texture, WGPUTexture, texture, internal::textureDestructor);

class Texture final : public TextureResourceHolder {
    Texture(WGPUTexture texture, WGPUTextureFormat format) {
        _texture = texture;
        _format = format;
    }

public:
    static Texture makeDepthTexture(Device const& device, WGPUTextureFormat format);

    WGPUTextureFormat format() const { return _format; }
private:
    WGPUTextureFormat _format;
};

} // namespace runtime::graphics_engine::webgpu
