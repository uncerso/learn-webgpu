#pragma once
#include <cstdint>

namespace runtime::graphics_engine {

enum class FrontFace : uint8_t {
    CCW,
    CW,
};

enum class PrimitiveTopology : uint8_t {
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,
};

enum class CullMode : uint8_t {
    None,
    Front,
    Back,
};

struct PipelineConfig {
    FrontFace frontFace = FrontFace::CCW;
    PrimitiveTopology topology = PrimitiveTopology::TriangleList;
    CullMode cullMode = CullMode::None;
};

} // namespace runtime::graphics_engine
