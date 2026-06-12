#include "renderer.hpp"

#include "stockfih/square.hpp"

namespace stockfih::gui {
namespace {

constexpr Color kLightSquare = {240, 217, 181, 255};
constexpr Color kDarkSquare = {181, 136, 99, 255};

}  // namespace

Vector2 squareTopLeft(const BoardLayout& layout, int file, int rank) {
  const float x = static_cast<float>(layout.originX + file * layout.squareSize);
  // Rank 0 at the bottom: invert the rank when converting to a screen row.
  const float y = static_cast<float>(
      layout.originY + (kBoardSize - 1 - rank) * layout.squareSize);
  return Vector2{x, y};
}

void drawBoardSquares(const BoardLayout& layout) {
  for (int rank = 0; rank < kBoardSize; ++rank) {
    for (int file = 0; file < kBoardSize; ++file) {
      const Vector2 topLeft = squareTopLeft(layout, file, rank);
      const bool light = (file + rank) % 2 != 0;  // a1 (0,0) is dark
      DrawRectangle(static_cast<int>(topLeft.x), static_cast<int>(topLeft.y),
                    layout.squareSize, layout.squareSize,
                    light ? kLightSquare : kDarkSquare);
    }
  }
}

}  // namespace stockfih::gui
