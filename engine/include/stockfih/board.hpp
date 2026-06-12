#pragma once

#include <array>
#include <cstdint>

#include "stockfih/piece.hpp"
#include "stockfih/square.hpp"

namespace stockfih {

// Castling availability stored as a bit mask.
enum CastlingRight : std::uint8_t {
  kNoCastling = 0,
  kWhiteKingSide = 1 << 0,
  kWhiteQueenSide = 1 << 1,
  kBlackKingSide = 1 << 2,
  kBlackQueenSide = 1 << 3,
  kAllCastling = kWhiteKingSide | kWhiteQueenSide | kBlackKingSide | kBlackQueenSide,
};

// Full chess position: the 8x8 grid plus the side state needed to fully
// describe a position (matching the fields of a FEN string).
class Board {
 public:
  [[nodiscard]] Piece at(Square square) const { return squares_[square]; }
  void set(Square square, Piece piece) { squares_[square] = piece; }

  [[nodiscard]] Color sideToMove() const { return side_to_move_; }
  void setSideToMove(Color color) { side_to_move_ = color; }

  [[nodiscard]] std::uint8_t castlingRights() const { return castling_; }
  void setCastlingRights(std::uint8_t rights) { castling_ = rights; }

  [[nodiscard]] Square enPassantSquare() const { return en_passant_; }
  void setEnPassantSquare(Square square) { en_passant_ = square; }

  [[nodiscard]] int halfmoveClock() const { return halfmove_clock_; }
  void setHalfmoveClock(int value) { halfmove_clock_ = value; }

  [[nodiscard]] int fullmoveNumber() const { return fullmove_number_; }
  void setFullmoveNumber(int value) { fullmove_number_ = value; }

  // Removes every piece, leaving the board empty (other state untouched).
  void clear();

  // The standard chess starting position.
  [[nodiscard]] static Board startingPosition();

 private:
  std::array<Piece, kNumSquares> squares_{};
  Color side_to_move_ = Color::White;
  std::uint8_t castling_ = kAllCastling;
  Square en_passant_ = kNoSquare;
  int halfmove_clock_ = 0;
  int fullmove_number_ = 1;
};

}  // namespace stockfih
