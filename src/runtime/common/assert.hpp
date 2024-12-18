#pragma once
#include <cstdlib>

#define ASSERT(v) do { if (!(v)) { std::abort(); } } while (false)
