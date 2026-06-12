#pragma once

namespace stockfih {

// Squares are indexed 0..63 with index = rank * 8 + file, where file 0 is the
// a-file and rank 0 is the first rank (a1 == 0, h1 == 7, a8 == 56, h8 == 63).
using Square = int;

inline constexpr int kBoardSize = 8;
inline constexpr int kNumSquares = 64;
inline constexpr Square kNoSquare = -1;

[[nodiscard]] constexpr int fileOf(Square square) { return square % kBoardSize; }
[[nodiscard]] constexpr int rankOf(Square square) { return square / kBoardSize; }

[[nodiscard]] constexpr Square makeSquare(int file, int rank) {
  return rank * kBoardSize + file;
}

[[nodiscard]] constexpr bool isValidSquare(Square square) {
  return square >= 0 && square < kNumSquares;
}

[[nodiscard]] constexpr bool isOnBoard(int file, int rank) {
  return file >= 0 && file < kBoardSize && rank >= 0 && rank < kBoardSize;
}

}  // namespace stockfih
