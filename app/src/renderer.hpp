#pragma once

#include "raylib.h"

namespace stockfih::gui {

// Where and how big the board is drawn on screen.
struct BoardLayout {
  int originX = 0;
  int originY = 0;
  int squareSize = 80;
};

// Pixel position of the top-left corner of board square (file, rank). Rank 0
// (White's first rank) is drawn at the bottom, matching how a player sees it.
[[nodiscard]] Vector2 squareTopLeft(const BoardLayout& layout, int file, int rank);

// Draws the 8x8 checkered board.
void drawBoardSquares(const BoardLayout& layout);

}  // namespace stockfih::gui
