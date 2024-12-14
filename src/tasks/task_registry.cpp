#include "task_registry.hpp"

namespace tasks {

namespace {

std::vector<TaskRegistry*> g_allRegistries;

} // namespace

TaskRegistry::TaskRegistry() {
    g_allRegistries.push_back(this);
}

std::vector<TaskRegistry*> const& TaskRegistry::tasks() {
    return g_allRegistries;
}


} // tasks