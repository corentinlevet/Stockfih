#include "stockfih/evaluation.hpp"

namespace stockfih {
namespace {

int pieceValue(PieceType type) {
  switch (type) {
    case PieceType::Pawn: return 100;
    case PieceType::Knight: return 320;
    case PieceType::Bishop: return 330;
    case PieceType::Rook: return 500;
    case PieceType::Queen: return 900;
    case PieceType::King:
    case PieceType::None: return 0;
  }
  return 0;
}

}  // namespace

int evaluate(const Board& board) {
  int score = 0;
  for (Square square = 0; square < kNumSquares; ++square) {
    const Piece piece = board.at(square);
    if (piece.isNone()) continue;
    const int value = pieceValue(piece.type);
    score += piece.color == Color::White ? value : -value;
  }
  return score;
}

}  // namespace stockfih
