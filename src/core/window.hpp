#pragma once

#include "renderer_creator.hpp"

#include <runtime/grapgics_engine/renderer.hpp>
#include <cstddef>
#include <optional>

struct GLFWwindow;

namespace core {

class Window {
    Window(size_t width, size_t height, GLFWwindow * window, std::unique_ptr<runtime::graphics_engine::IRenderer> renderer);

    Window(Window const &) = delete;
    Window(Window &&) = delete;

    Window & operator = (Window const &) = delete;
    Window & operator = (Window &&) = delete;
public:
    enum class ExitReason {
        Quit, RequestedPrev, RequestedNext
    };

    static Window & create(size_t width, size_t height);
    static Window & get();

    ~Window();

    ExitReason render(RendererCreator creator);

    void stopRendering(ExitReason);

    const size_t width;
    const size_t height;
private:
    std::unique_ptr<runtime::graphics_engine::IRenderer> _renderer;
    GLFWwindow * _window;
    std::optional<ExitReason> _exitReason;
};

} // namespace core
