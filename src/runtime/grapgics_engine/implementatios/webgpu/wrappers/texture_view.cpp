#include "texture_view.hpp"

namespace runtime::graphics_engine::webgpu {

namespace {

WGPUTextureView nextSurfaceTextureView(Surface const& surface) {
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(surface.surface(), &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success)
        return nullptr;

    WGPUTextureViewDescriptor viewDescriptor {
        .nextInChain = nullptr,
        .label = "Surface texture view",
        .format = wgpuTextureGetFormat(surfaceTexture.texture),
        .dimension = WGPUTextureViewDimension_2D,
        .baseMipLevel = 0,
        .mipLevelCount = 1,
        .baseArrayLayer = 0,
        .arrayLayerCount = 1,
        .aspect = WGPUTextureAspect_All,
    };
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

    return {TextureView{view}};
}

TextureView TextureView::create2DViewDepth(Texture const& texture) {
    WGPUTextureViewDescriptor depthTextureViewDesc {
        .nextInChain = nullptr,
        .label = nullptr,
        .format = texture.format(),
        .dimension = WGPUTextureViewDimension_2D,
        .baseMipLevel = 0,
        .mipLevelCount = 1,
        .baseArrayLayer = 0,
        .arrayLayerCount = 1,
        .aspect = WGPUTextureAspect_DepthOnly,
    };
    WGPUTextureView depthTextureView = wgpuTextureCreateView(texture.texture(), &depthTextureViewDesc);
    return TextureView{depthTextureView};
}

} // namespace runtime::graphics_engine::webgpu
