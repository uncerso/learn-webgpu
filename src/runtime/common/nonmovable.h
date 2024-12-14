#pragma once

namespace runtime {

struct NonMovable {
    constexpr NonMovable(NonMovable&&) noexcept = delete;
    constexpr NonMovable& operator=(NonMovable&&) noexcept = delete;

    constexpr NonMovable(const NonMovable&) = delete;
    constexpr NonMovable& operator=(const NonMovable&) = delete;

    constexpr NonMovable(const NonMovable&&) = delete;
    constexpr NonMovable& operator=(const NonMovable&&) = delete;

    constexpr NonMovable() = default;
    constexpr ~NonMovable() = default;
};

} // namespace runtime
