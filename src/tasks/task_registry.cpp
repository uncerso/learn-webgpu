#include "task_registry.hpp"
#include <algorithm>

namespace tasks {

namespace {

std::vector<TaskRegistry*> g_allRegistries;

} // namespace

TaskRegistry::TaskRegistry() {
    g_allRegistries.push_back(this);
}

std::vector<TaskRegistry*> const& TaskRegistry::tasks() {
    std::sort(g_allRegistries.begin(), g_allRegistries.end(), [](auto const* lhs, auto const* rhs) { return lhs->name() < rhs->name(); });
    return g_allRegistries;
}


} // namespace tasks