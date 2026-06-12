#include "stockfih/evaluation.hpp"

#include "stockfih/movegen.hpp"

namespace stockfih {
namespace {

constexpr int kCenterBonus = 25;    // per central square occupied
constexpr int kMobilityWeight = 5;  // per extra available move

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

// Number of pseudo-legal moves available to `side`, independent of whose turn it
// actually is.
int moveCount(const Board& board, Color side) {
  Board view = board;
  view.setSideToMove(side);
  return static_cast<int>(generatePseudoLegalMoves(view).size());
}

}  // namespace

int materialBalance(const Board& board) {
  int score = 0;
  for (Square square = 0; square < kNumSquares; ++square) {
    const Piece piece = board.at(square);
    if (piece.isNone()) continue;
    const int value = pieceValue(piece.type);
    score += piece.color == Color::White ? value : -value;
  }
  return score;
}

int centerControl(const Board& board) {
  const Square centers[] = {makeSquare(3, 3), makeSquare(4, 3), makeSquare(3, 4),
                            makeSquare(4, 4)};  // d4, e4, d5, e5
  int score = 0;
  for (const Square square : centers) {
    const Piece piece = board.at(square);
    if (piece.isNone()) continue;
    score += piece.color == Color::White ? kCenterBonus : -kCenterBonus;
  }
  return score;
}

int mobilityBalance(const Board& board) {
  return kMobilityWeight * (moveCount(board, Color::White) - moveCount(board, Color::Black));
}

int evaluate(const Board& board) {
  return materialBalance(board) + centerControl(board) + mobilityBalance(board);
}

}  // namespace stockfih
