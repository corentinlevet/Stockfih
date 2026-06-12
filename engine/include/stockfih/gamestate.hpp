#pragma once

#include "stockfih/board.hpp"
#include "stockfih/move.hpp"

namespace stockfih {

// Applies a move to a board and returns the resulting position. The move is
// assumed to be (pseudo-)legal. Special moves are recognized from context:
//   - Castling:   the king moves two files; the matching rook is moved too.
//   - En passant: a pawn moves diagonally onto the en-passant target square;
//                 the passed pawn is removed.
//   - Promotion:  move.promotion is non-None; the pawn becomes that piece.
// Side to move, castling rights, en-passant square, and the move counters are
// all updated.
[[nodiscard]] Board makeMove(const Board& board, const Move& move);

}  // namespace stockfih
