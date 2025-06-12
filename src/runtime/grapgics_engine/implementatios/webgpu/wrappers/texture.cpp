#include "texture.hpp"

namespace runtime::graphics_engine::webgpu {

namespace internal {

void textureDestructor(WGPUTexture texture) {
    wgpuTextureDestroy(texture);
    wgpuTextureRelease(texture);
}

} // internal

Texture Texture::makeDepthTexture(Device const& device, WGPUTextureFormat format) {
    WGPUTextureDescriptor depthTextureDesc {
        .nextInChain = nullptr,
        .label = nullptr,
        .usage = WGPUTextureUsage_RenderAttachment,
        .dimension = WGPUTextureDimension_2D,
        .size = {640, 480, 1},
        .format = format,
        .mipLevelCount = 1,
        .sampleCount = 1,
        .viewFormatCount = 1,
        .viewFormats = &format,
    };
    WGPUTexture texture = wgpuDeviceCreateTexture(device.device(), &depthTextureDesc);
    return Texture{ texture, format };
}

} // namespace runtime::graphics_engine::webgpu
