#include "core/window.hpp"
#include "tasks/task_registry.hpp"

#include <iostream>

int main() try {
    auto& window = core::Window::create(640, 480);

    auto& tasks = tasks::TaskRegistry::tasks();
    if (!tasks.empty()) {
        auto* task = tasks.back();
        std::cout << "Selected task: '" << task->name() << "'" << std::endl;
        window.render(task->creator());
    }

    return 0;
} catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
} catch (std::string const & e) {
    std::cerr << "Error: " << e << std::endl;
    return 1;
} catch (char const * e) {
    std::cerr << "Error: " << e << std::endl;
    return 1;
} catch (...) {
    std::cerr << "Unknown error :(" << std::endl;
    return 2;
}
