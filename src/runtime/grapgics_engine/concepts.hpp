#include <cstdint>
#include <concepts>

template <typename T>
concept SupportedIndex = std::same_as<T, uint16_t> || std::same_as<T, uint32_t>;
