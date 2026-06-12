#include <gtest/gtest.h>

#include "stockfih/piece.hpp"

namespace stockfih {
namespace {

TEST(Piece, DefaultIsNone) {
  constexpr Piece piece;
  EXPECT_TRUE(piece.isNone());
  EXPECT_EQ(piece, kNoPiece);
}

TEST(Piece, Equality) {
  constexpr Piece white_pawn{PieceType::Pawn, Color::White};
  constexpr Piece black_pawn{PieceType::Pawn, Color::Black};
  EXPECT_EQ(white_pawn, (Piece{PieceType::Pawn, Color::White}));
  EXPECT_NE(white_pawn, black_pawn);
  EXPECT_FALSE(white_pawn.isNone());
}

TEST(Piece, OppositeColor) {
  EXPECT_EQ(opposite(Color::White), Color::Black);
  EXPECT_EQ(opposite(Color::Black), Color::White);
}

}  // namespace
}  // namespace stockfih
