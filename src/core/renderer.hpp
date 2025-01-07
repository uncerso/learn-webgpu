#pragma once

#include <cstddef>
#include <runtime/grapgics_engine/renderer.hpp>

#include <glm/glm.hpp>

namespace core {

class Renderer {
public:
    struct FrameContext {
        runtime::graphics_engine::IRenderer& renderer;
        float renderTime;
    };
    virtual ~Renderer() = default;
    virtual void render(FrameContext const& context) = 0;

    virtual glm::vec4 bgColor() const noexcept { return {0.0, 0.0, 0.0, 1.0}; }

    // float widthHeightRatio() const noexcept { return float(double(width) / double(height)); }
    // size_t width = 1;
    // size_t height = 1;
};

} // namespace core
