#pragma once

#include "stockfih/piece.hpp"
#include "stockfih/square.hpp"

namespace stockfih {

// A move from one square to another. `promotion` is the piece a pawn becomes on
// reaching the last rank (PieceType::None for ordinary moves). Special-move
// flags (castling, en passant) are added in a later issue.
struct Move {
  Square from = kNoSquare;
  Square to = kNoSquare;
  PieceType promotion = PieceType::None;

  friend constexpr bool operator==(const Move&, const Move&) = default;
};

}  // namespace stockfih
