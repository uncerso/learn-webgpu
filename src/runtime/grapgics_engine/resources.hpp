#pragma once

#include <runtime/common/nonmovable.h>

namespace runtime::graphics_engine {

struct IMesh : NonMovable {
    virtual ~IMesh() = default;
};

struct IRenderPipeline : NonMovable {
    virtual ~IRenderPipeline() = default;
};

} // runtime::graphics_engine
