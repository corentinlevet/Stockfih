#pragma once

#include <string_view>

namespace stockfih {

// Returns the engine version string (matches the CMake project version).
[[nodiscard]] std::string_view version() noexcept;

}  // namespace stockfih
