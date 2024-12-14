#pragma once

#include "renderer.hpp"
#include <memory>

namespace core {

using RendererCreator = std::unique_ptr<core::Renderer>(*)(runtime::graphics_engine::IRenderer&);

} // namespace core
