#pragma once

#include "resource_manager.hpp"
#include "render_pass_encoder.hpp"
#include <memory>

struct GLFWwindow;

namespace runtime::graphics_engine {

struct IRenderer : NonMovable {
    virtual ~IRenderer() = default;

    virtual IResourceManager& resourceManager() = 0;
    virtual IRenderPassEncoder& renderPassEncoder() = 0;

    virtual bool tryStartFrame() = 0;
    virtual void finishFrame() = 0;
};

std::unique_ptr<IRenderer> createRenderer(uint32_t width, uint32_t height, GLFWwindow& window);

} // namespace runtime::graphics_engine
