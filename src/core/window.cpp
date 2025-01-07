#include "window.hpp"

#include <runtime/common/exception.hpp>
// #include <runtime/common/assert.hpp>

#include <GLFW/glfw3.h>

#include <memory>
#include <string>

// #include <webgpu/webgpu.h>

// #include <glfw3webgpu.h>

// #ifdef WEBGPU_BACKEND_WGPU
// #  include <webgpu/wgpu.h>
// #endif // WEBGPU_BACKEND_WGPU

namespace core {

namespace {
std::unique_ptr<Window> windowSingleton;

void error_callback(int error, char const * msg) {
    using namespace std::string_literals;
    throw " ["s + std::to_string(error) + "] "s + msg;
}

template <typename T>
struct PointerGuard {
    T * & p;
    PointerGuard(T * & p) : p(p) {}
    PointerGuard(T * & p, T * val) : PointerGuard(p) { p = val; }
    ~PointerGuard() { p = nullptr; }
};

} // namespace

Window& Window::create(size_t width, size_t height) {
    glfwSetErrorCallback(error_callback);

    REQUIRE(glfwInit() == GLFW_TRUE, "Could not initialize GLFW");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    auto* window = glfwCreateWindow(static_cast<GLint>(width), static_cast<GLint>(height), "Learn WebGPU", nullptr, nullptr);
    REQUIRE(window, "Failed to create GLFW window");

    // glfwSetKeyCallback(window, keyCallback);
    // glfwSetCursorPosCallback(window, mouseCallback);

    auto renderer = runtime::graphics_engine::createRenderer(static_cast<uint32_t>(width), static_cast<uint32_t>(height), *window);
    windowSingleton.reset(new Window(width, height, window, std::move(renderer)));
    return *windowSingleton;
}

Window& Window::get() {
    REQUIRE(windowSingleton, "You have to create window before 'Window::get' can be called");
    return *windowSingleton;
}

void Window::stopRendering(ExitReason reason) {
    _exitReason.emplace(reason);
}

Window::Window(size_t width, size_t height, GLFWwindow * window, std::unique_ptr<runtime::graphics_engine::IRenderer> renderer)
    : width(width)
    , height(height)
    , _renderer(std::move(renderer))
    , _window(window)
{}

Window::~Window() {
    if (_window) {
        glfwSetWindowShouldClose(_window, GL_TRUE);
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
    glfwTerminate();
}

Window::ExitReason Window::render(RendererCreator creator) {
    _exitReason.reset();

    auto renderer = creator(*_renderer);
    // int w, h;
    // glfwGetFramebufferSize(window, &w, &h);

    while (!_exitReason && !glfwWindowShouldClose(_window)) {
        float renderTime = float(glfwGetTime());
        glfwPollEvents();

        if (!_renderer->tryStartFrame(renderer->bgColor()))
            continue;

        renderer->render({
            .renderer = *_renderer,
            .renderTime = renderTime,
        });

        _renderer->finishFrame();
    }

    return *_exitReason;
}

} // namespace core
