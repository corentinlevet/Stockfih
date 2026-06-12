#pragma once

#include "stockfih/board.hpp"

namespace stockfih {

// Static evaluation of a position in centipawns from White's perspective
// (positive favors White, negative favors Black).
[[nodiscard]] int evaluate(const Board& board);

}  // namespace stockfih
