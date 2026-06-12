#pragma once

#include <vector>

#include "stockfih/board.hpp"
#include "stockfih/move.hpp"

namespace stockfih {

// Generates the pseudo-legal moves for the side to move. "Pseudo-legal" means
// the moves follow each piece's movement rules but king safety (leaving/placing
// the own king in check) is not yet validated -- that is handled in issue #6.
[[nodiscard]] std::vector<Move> generatePseudoLegalMoves(const Board& board);

}  // namespace stockfih
