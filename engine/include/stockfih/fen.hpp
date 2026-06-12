#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "stockfih/board.hpp"

namespace stockfih {

// Forsyth-Edwards Notation for the standard starting position.
inline constexpr std::string_view kStartingFen =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// Parses a FEN string into a Board. Returns std::nullopt when the string is
// malformed (wrong field count, bad ranks, invalid characters, ...).
[[nodiscard]] std::optional<Board> parseFen(std::string_view fen);

// Serializes a Board back into a FEN string.
[[nodiscard]] std::string toFen(const Board& board);

}  // namespace stockfih
