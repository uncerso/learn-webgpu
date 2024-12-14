#pragma once

#include <runtime/common/move_only.h>

namespace runtime::graphics_engine::webgpu {


#define RESOURCE_HOLDER(ClassName, ResourceType, resourceName, destructor)  \
struct ClassName##ResourceHolder : MoveOnly {                               \
public:                                                                     \
    ClassName##ResourceHolder(ResourceType resourceName = nullptr)          \
        : _##resourceName(resourceName)                                     \
    {}                                                                      \
    ~ClassName##ResourceHolder() { reset(); }                               \
    ClassName##ResourceHolder(ClassName##ResourceHolder&& other)            \
        : _##resourceName(std::exchange(other._##resourceName, nullptr))    \
    {}                                                                      \
    void operator=(ClassName##ResourceHolder&& other) {                     \
        if (&other == this) return;                                         \
        reset();                                                            \
        _##resourceName = std::exchange(other._##resourceName, nullptr);    \
    }                                                                       \
    ResourceType const& resourceName() const noexcept {                     \
        return _##resourceName;                                             \
    }                                                                       \
protected:                                                                  \
    ResourceType _##resourceName = nullptr;                                 \
                                                                            \
private:                                                                    \
    void reset() {                                                          \
        if (_##resourceName) {                                              \
            destructor(_##resourceName);                                    \
            _##resourceName = nullptr;                                      \
        }                                                                   \
    }                                                                       \
}

} // runtime::graphics_engine::webgpu
