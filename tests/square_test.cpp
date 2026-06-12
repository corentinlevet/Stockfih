#include <gtest/gtest.h>

#include "stockfih/square.hpp"

namespace stockfih {
namespace {

TEST(Square, MakeSquareRoundTrips) {
  for (int rank = 0; rank < kBoardSize; ++rank) {
    for (int file = 0; file < kBoardSize; ++file) {
      const Square square = makeSquare(file, rank);
      EXPECT_EQ(fileOf(square), file);
      EXPECT_EQ(rankOf(square), rank);
    }
  }
}

TEST(Square, KnownCorners) {
  EXPECT_EQ(makeSquare(0, 0), 0);    // a1
  EXPECT_EQ(makeSquare(7, 0), 7);    // h1
  EXPECT_EQ(makeSquare(0, 7), 56);   // a8
  EXPECT_EQ(makeSquare(7, 7), 63);   // h8
}

TEST(Square, Validity) {
  EXPECT_TRUE(isValidSquare(0));
  EXPECT_TRUE(isValidSquare(63));
  EXPECT_FALSE(isValidSquare(-1));
  EXPECT_FALSE(isValidSquare(64));
  EXPECT_FALSE(isValidSquare(kNoSquare));
}

TEST(Square, OnBoard) {
  EXPECT_TRUE(isOnBoard(0, 0));
  EXPECT_TRUE(isOnBoard(7, 7));
  EXPECT_FALSE(isOnBoard(-1, 0));
  EXPECT_FALSE(isOnBoard(8, 0));
  EXPECT_FALSE(isOnBoard(0, 8));
}

}  // namespace
}  // namespace stockfih
