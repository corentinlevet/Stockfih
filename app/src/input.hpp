#pragma once

#include "raylib.h"
#include "renderer.hpp"
#include "stockfih/square.hpp"

namespace stockfih::gui {

// The board square under a pixel position, or kNoSquare when the pixel is
// outside the board area.
[[nodiscard]] Square squareAtPixel(const BoardLayout& layout, Vector2 pixel);

}  // namespace stockfih::gui
