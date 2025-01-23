#pragma once

#include <cstdint>
#include <functional>

using Milliseconds = uint64_t;
using RenderFuncDelegate = std::function<void(Milliseconds now, Milliseconds delta_ms)>;
using UpdateFuncDelegate = std::function<void(Milliseconds now, Milliseconds delta_ms)>;

using WindowId = uint32_t;
