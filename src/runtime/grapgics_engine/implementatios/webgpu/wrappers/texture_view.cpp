#include "texture_view.hpp"

namespace runtime::graphics_engine::webgpu {

namespace {

WGPUTextureView nextSurfaceTextureView(Surface const& surface) {
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(surface.surface(), &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success)
        return nullptr;

    WGPUTextureViewDescriptor viewDescriptor;
    viewDescriptor.nextInChain = nullptr;
    viewDescriptor.label = "Surface texture view";
    viewDescriptor.format = wgpuTextureGetFormat(surfaceTexture.texture);
    viewDescriptor.dimension = WGPUTextureViewDimension_2D;
    viewDescriptor.baseMipLevel = 0;
    viewDescriptor.mipLevelCount = 1;
    viewDescriptor.baseArrayLayer = 0;
    viewDescriptor.arrayLayerCount = 1;
    viewDescriptor.aspect = WGPUTextureAspect_All;
    WGPUTextureView targetView = wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);

    // (NB: with wgpu-native, surface textures must not be manually released)
    // wgpuTextureRelease(surfaceTexture.texture);

    return targetView;
}

} // namespace

std::optional<TextureView> TextureView::tryCreate(Surface const& surface) {
    auto view = nextSurfaceTextureView(surface);
    if (!view)
        return std::nullopt;

    return {{view}};
}

} // namespace runtime::graphics_engine::webgpu
