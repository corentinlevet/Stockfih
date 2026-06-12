#pragma once

#include <vector>

#include "raylib.h"
#include "stockfih/board.hpp"
#include "stockfih/move.hpp"

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

// Draws file letters (a-h) below and rank numbers (1-8) to the left of the board.
void drawCoordinates(const BoardLayout& layout);

// Draws every piece on `board` at its square. Pieces are placeholder discs
// labelled with the piece letter (P/N/B/R/Q/K), colored by side; real sprite
// textures can replace this later without touching the rest of the GUI.
void drawPieces(const BoardLayout& layout, const Board& board);

// Outlines the selected square (if any) and marks each move's destination with
// a dot, so the player sees where the selected piece may go.
void drawHighlights(const BoardLayout& layout, Square selected,
                    const std::vector<Move>& moves);

// Draws a status line (e.g. "Checkmate") below the board.
void drawStatus(const BoardLayout& layout, const char* text);

}  // namespace stockfih::gui
