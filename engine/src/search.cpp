#include "stockfih/search.hpp"

#include <algorithm>
#include <limits>
#include <vector>

#include "stockfih/gamestate.hpp"
#include "stockfih/rules.hpp"

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

// Score for a position with no legal moves: a loss for the side to move when in
// check (mate), otherwise a draw (stalemate). `ply` makes nearer mates better.
int terminalScore(const Board& board, int ply) {
  const Color stm = board.sideToMove();
  if (!isInCheck(board, stm)) return 0;
  const int mate = kMateScore - ply;
  return stm == Color::White ? -mate : mate;
}

int search(const Board& board, int depth, int ply) {
  const std::vector<Move> legal = generateLegalMoves(board);
  if (legal.empty()) return terminalScore(board, ply);
  if (depth == 0) return evaluate(board);

  const bool maximizing = board.sideToMove() == Color::White;
  int best = maximizing ? std::numeric_limits<int>::min()
                        : std::numeric_limits<int>::max();
  for (const Move& move : legal) {
    const int score = search(makeMove(board, move), depth - 1, ply + 1);
    best = maximizing ? std::max(best, score) : std::min(best, score);
  }
  return best;
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

int minimax(const Board& board, int depth) { return search(board, depth, 0); }

}  // namespace stockfih
