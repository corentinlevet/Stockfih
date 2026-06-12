#pragma once

#include "stockfih/board.hpp"
#include "stockfih/move.hpp"

namespace stockfih {

// Score returned for a forced mate. Faster mates score slightly higher so the
// search prefers them.
inline constexpr int kMateScore = 1'000'000;

// Static evaluation of a position in centipawns from White's perspective
// (positive favors White). Material only for now; refined in issue #9.
[[nodiscard]] int evaluate(const Board& board);

// Minimax value of the position searched to `depth` plies, from White's
// perspective. Equivalent to a full minimax (no pruning).
[[nodiscard]] int minimax(const Board& board, int depth);

// Same value as minimax(), computed with alpha-beta pruning.
[[nodiscard]] int alphaBeta(const Board& board, int depth);

// Result of an instrumented search: the score and the number of nodes visited.
struct SearchStats {
  int score = 0;
  long long nodes = 0;
};

// Searches to `depth` plies, with or without alpha-beta pruning, reporting how
// many nodes were visited. Used to verify pruning yields the same score while
// visiting fewer nodes.
[[nodiscard]] SearchStats searchWithStats(const Board& board, int depth,
                                          bool useAlphaBeta);

// The best move for the side to move, searched to `depth` plies using
// alpha-beta pruning. Returns a default-constructed Move when there is no legal
// move.
[[nodiscard]] Move findBestMove(const Board& board, int depth);

}  // namespace stockfih
