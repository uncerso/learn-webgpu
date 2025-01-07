#pragma once
#include <cstdlib>
#include <iostream>

#define ASSERT(v) do { if (!(v)) { std::cerr << "Assertion '" << #v << "' failed at " << __FILE__ << ":" << __LINE__ << std::endl; std::abort(); } } while (false)
