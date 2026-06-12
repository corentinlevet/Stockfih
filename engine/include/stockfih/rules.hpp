#pragma once

#include <vector>

#include "stockfih/board.hpp"
#include "stockfih/move.hpp"

namespace stockfih {

// True if `square` is attacked by any piece of color `byColor`.
[[nodiscard]] bool isSquareAttacked(const Board& board, Square square, Color byColor);

// Returns the square of `color`'s king, or kNoSquare if there is none.
[[nodiscard]] Square findKing(const Board& board, Color color);

// True if `color`'s king is currently under attack.
[[nodiscard]] bool isInCheck(const Board& board, Color color);

// The fully legal moves for the side to move: pseudo-legal moves that do not
// leave the mover's king in check, with castling additionally forbidden out of,
// through, or into check.
[[nodiscard]] std::vector<Move> generateLegalMoves(const Board& board);

// True if the side to move is in check and has no legal move.
[[nodiscard]] bool isCheckmate(const Board& board);

// True if the side to move is not in check but has no legal move.
[[nodiscard]] bool isStalemate(const Board& board);

}  // namespace stockfih
