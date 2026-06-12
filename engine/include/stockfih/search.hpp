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
// perspective.
[[nodiscard]] int minimax(const Board& board, int depth);

// The best move for the side to move, searched to `depth` plies. Returns a
// default-constructed Move when there is no legal move.
[[nodiscard]] Move findBestMove(const Board& board, int depth);

}  // namespace stockfih
