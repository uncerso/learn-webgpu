#pragma once

#include <cstddef>
#include <runtime/grapgics_engine/renderer.hpp>

namespace core {

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(runtime::graphics_engine::IRenderer& renderer) = 0;

    // float widthHeightRatio() const noexcept { return float(double(width) / double(height)); }
    // size_t width = 1;
    // size_t height = 1;
};

} // namespace core
