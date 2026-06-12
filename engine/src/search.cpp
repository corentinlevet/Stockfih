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

// Depth-limited minimax (White maximizes, Black minimizes). When `prune` is
// true, alpha-beta cutoffs skip branches that cannot affect the result; the
// returned score is identical either way. `nodes` counts visited nodes.
int search(const Board& board, int depth, int ply, int alpha, int beta,
           bool prune, long long& nodes) {
  ++nodes;
  const std::vector<Move> legal = generateLegalMoves(board);
  if (legal.empty()) return terminalScore(board, ply);
  if (depth == 0) return evaluate(board);

  if (board.sideToMove() == Color::White) {
    int best = std::numeric_limits<int>::min();
    for (const Move& move : legal) {
      best = std::max(
          best, search(makeMove(board, move), depth - 1, ply + 1, alpha, beta, prune,
                       nodes));
      if (prune) {
        alpha = std::max(alpha, best);
        if (alpha >= beta) break;  // Black would avoid this line
      }
    }
    return best;
  }

  int best = std::numeric_limits<int>::max();
  for (const Move& move : legal) {
    best = std::min(
        best, search(makeMove(board, move), depth - 1, ply + 1, alpha, beta, prune,
                     nodes));
    if (prune) {
      beta = std::min(beta, best);
      if (beta <= alpha) break;  // White would avoid this line
    }
  }
  return best;
}

constexpr int kInf = std::numeric_limits<int>::max();

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

int minimax(const Board& board, int depth) {
  long long nodes = 0;
  return search(board, depth, 0, -kInf, kInf, /*prune=*/false, nodes);
}

int alphaBeta(const Board& board, int depth) {
  long long nodes = 0;
  return search(board, depth, 0, -kInf, kInf, /*prune=*/true, nodes);
}

SearchStats searchWithStats(const Board& board, int depth, bool useAlphaBeta) {
  SearchStats stats;
  stats.score =
      search(board, depth, 0, -kInf, kInf, useAlphaBeta, stats.nodes);
  return stats;
}

Move findBestMove(const Board& board, int depth) {
  const std::vector<Move> legal = generateLegalMoves(board);
  if (legal.empty()) return Move{};

  const bool maximizing = board.sideToMove() == Color::White;
  Move best = legal.front();
  int bestScore = maximizing ? -kInf : kInf;
  long long nodes = 0;
  int alpha = -kInf;
  int beta = kInf;
  for (const Move& move : legal) {
    const int score =
        search(makeMove(board, move), depth - 1, 1, alpha, beta, /*prune=*/true,
               nodes);
    if (maximizing ? score > bestScore : score < bestScore) {
      bestScore = score;
      best = move;
    }
    // Narrow the window at the root as the best score improves.
    if (maximizing) {
      alpha = std::max(alpha, bestScore);
    } else {
      beta = std::min(beta, bestScore);
    }
  }
  return best;
}

}  // namespace stockfih
