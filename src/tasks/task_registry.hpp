#pragma once

#include <core/renderer_creator.hpp>
#include <vector>
#include <string_view>

namespace tasks {

struct TaskRegistry {
    using RendererCreator = core::RendererCreator;

    TaskRegistry();
    virtual ~TaskRegistry() = default;

    static std::vector<TaskRegistry*> const& tasks();

    virtual std::string_view name() const noexcept = 0;
    virtual RendererCreator creator() const noexcept = 0;
};

} // namespace tasks
