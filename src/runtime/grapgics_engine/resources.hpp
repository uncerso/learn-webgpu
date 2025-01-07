#pragma once

#include <runtime/common/nonmovable.h>
#include <span>

namespace runtime::graphics_engine {

struct IMesh : NonMovable {
    virtual ~IMesh() = default;
};

struct IRenderPipeline : NonMovable {
    virtual ~IRenderPipeline() = default;
};

struct IUniformBuffer : NonMovable {
    virtual ~IUniformBuffer() = default;
protected:
    virtual void rewriteImpl(std::span<std::byte const> bytes, size_t startPos = 0) = 0;

    template <typename Vertex>
    void rewrite(Vertex const& data) {
        return rewriteImpl(std::span{reinterpret_cast<std::byte const*>(&data), sizeof(Vertex)});
    }
};

template <typename Vertex>
struct ITUniformBuffer : IUniformBuffer {
    friend inline void rewrite(ITUniformBuffer<Vertex>& self, Vertex const& data) {
        return self.rewrite(data);
    }
};

} // namespace runtime::graphics_engine
