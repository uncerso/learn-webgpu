#pragma once

namespace runtime {

struct MoveOnly {
    constexpr MoveOnly(MoveOnly&&) noexcept = default;
    constexpr MoveOnly& operator=(MoveOnly&&) noexcept = default;

    constexpr MoveOnly(const MoveOnly&) = delete;
    constexpr MoveOnly& operator=(const MoveOnly&) = delete;

    constexpr MoveOnly(const MoveOnly&&) = delete;
    constexpr MoveOnly& operator=(const MoveOnly&&) = delete;

    constexpr MoveOnly() = default;
    constexpr ~MoveOnly() = default;
};

} // namespace runtime
