#include "input.hpp"

namespace stockfih::gui {

Square squareAtPixel(const BoardLayout& layout, Vector2 pixel) {
  const int relX = static_cast<int>(pixel.x) - layout.originX;
  const int relY = static_cast<int>(pixel.y) - layout.originY;
  if (relX < 0 || relY < 0) return kNoSquare;

  const int file = relX / layout.squareSize;
  const int row = relY / layout.squareSize;
  if (file >= kBoardSize || row >= kBoardSize) return kNoSquare;

  // Row 0 is the top of the screen, which is rank 7 (the 8th rank).
  return makeSquare(file, kBoardSize - 1 - row);
}

}  // namespace stockfih::gui
