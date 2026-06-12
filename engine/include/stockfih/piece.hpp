#pragma once

#include <cstdint>

namespace stockfih {

enum class Color : std::uint8_t { White, Black };

enum class PieceType : std::uint8_t {
  None,
  Pawn,
  Knight,
  Bishop,
  Rook,
  Queen,
  King,
};

struct Piece {
  PieceType type = PieceType::None;
  Color color = Color::White;

  [[nodiscard]] constexpr bool isNone() const { return type == PieceType::None; }

  friend constexpr bool operator==(const Piece&, const Piece&) = default;
};

inline constexpr Piece kNoPiece{};

[[nodiscard]] constexpr Color opposite(Color color) {
  return color == Color::White ? Color::Black : Color::White;
}

}  // namespace stockfih
