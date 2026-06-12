#pragma once

#include "stockfih/board.hpp"

namespace stockfih {

// Material balance in centipawns (White minus Black).
[[nodiscard]] int materialBalance(const Board& board);

// Bonus for occupying the four central squares (d4/e4/d5/e5), White minus Black.
[[nodiscard]] int centerControl(const Board& board);

// Mobility term: a weighted difference between the number of pseudo-legal moves
// available to White and to Black.
[[nodiscard]] int mobilityBalance(const Board& board);

// Static evaluation of a position in centipawns from White's perspective
// (positive favors White, negative favors Black). Combines the terms above.
[[nodiscard]] int evaluate(const Board& board);

}  // namespace stockfih
