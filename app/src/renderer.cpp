#include "renderer.hpp"

#include "stockfih/square.hpp"

namespace stockfih::gui {
namespace {

// Raylib's Color struct is referenced as ::Color throughout because the engine's
// stockfih::Color enum is also in scope here. We use explicit literals rather
// than raylib's color macros (which expand to the bare token `Color`).
constexpr ::Color kLightSquare = {240, 217, 181, 255};
constexpr ::Color kDarkSquare = {181, 136, 99, 255};
constexpr ::Color kLabelColor = {60, 60, 60, 255};
constexpr ::Color kWhitePiece = {245, 245, 245, 255};
constexpr ::Color kBlackPiece = {30, 30, 30, 255};
constexpr int kLabelFontSize = 20;

void drawCharacter(char label, int x, int y) {
  const char text[2] = {label, '\0'};
  DrawText(text, x, y, kLabelFontSize, kLabelColor);
}

char pieceLetter(PieceType type) {
  switch (type) {
    case PieceType::Pawn: return 'P';
    case PieceType::Knight: return 'N';
    case PieceType::Bishop: return 'B';
    case PieceType::Rook: return 'R';
    case PieceType::Queen: return 'Q';
    case PieceType::King: return 'K';
    case PieceType::None: return ' ';
  }
  return ' ';
}

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

void drawCoordinates(const BoardLayout& layout) {
  const int half = layout.squareSize / 2;
  const int boardPixels = kBoardSize * layout.squareSize;

  for (int file = 0; file < kBoardSize; ++file) {
    const int x = layout.originX + file * layout.squareSize + half - 5;
    const int y = layout.originY + boardPixels + 6;
    drawCharacter(static_cast<char>('a' + file), x, y);
  }

  for (int rank = 0; rank < kBoardSize; ++rank) {
    const int x = layout.originX - 18;
    const int y =
        layout.originY + (kBoardSize - 1 - rank) * layout.squareSize + half - 10;
    drawCharacter(static_cast<char>('1' + rank), x, y);
  }
}

void drawPieces(const BoardLayout& layout, const Board& board) {
  const float radius = static_cast<float>(layout.squareSize) * 0.38f;
  const int fontSize = layout.squareSize / 2;

  for (Square square = 0; square < kNumSquares; ++square) {
    const Piece piece = board.at(square);
    if (piece.isNone()) continue;

    const Vector2 topLeft = squareTopLeft(layout, fileOf(square), rankOf(square));
    const Vector2 center{topLeft.x + layout.squareSize / 2.0f,
                         topLeft.y + layout.squareSize / 2.0f};

    const bool white = piece.color == stockfih::Color::White;
    const ::Color disc = white ? kWhitePiece : kBlackPiece;
    const ::Color outline = white ? kBlackPiece : kWhitePiece;
    const ::Color label = white ? kBlackPiece : kWhitePiece;

    DrawCircleV(center, radius, disc);
    DrawCircleLinesV(center, radius, outline);

    const char text[2] = {pieceLetter(piece.type), '\0'};
    const int textWidth = MeasureText(text, fontSize);
    DrawText(text, static_cast<int>(center.x) - textWidth / 2,
             static_cast<int>(center.y) - fontSize / 2, fontSize, label);
  }
}

}  // namespace stockfih::gui
