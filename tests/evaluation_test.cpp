#include <gtest/gtest.h>

#include "stockfih/evaluation.hpp"
#include "stockfih/fen.hpp"

namespace stockfih {
namespace {

Board boardFromFen(std::string_view fen) {
  const std::optional<Board> board = parseFen(fen);
  EXPECT_TRUE(board.has_value()) << "bad FEN: " << fen;
  return board.value_or(Board{});
}

TEST(Material, BalancedAtStartAndSignedByOwner) {
  EXPECT_EQ(materialBalance(Board::startingPosition()), 0);
  EXPECT_EQ(materialBalance(boardFromFen("7k/8/8/8/8/8/8/Q6K w - - 0 1")), 900);
  EXPECT_EQ(materialBalance(boardFromFen("r6k/8/8/8/8/8/8/7K w - - 0 1")), -500);
}

TEST(CenterControl, RewardsCentralOccupation) {
  EXPECT_GT(centerControl(boardFromFen("8/8/8/8/3N4/8/8/8 w - - 0 1")), 0);  // Nd4
  EXPECT_LT(centerControl(boardFromFen("8/8/8/3n4/8/8/8/8 w - - 0 1")), 0);  // ...Nd5
  EXPECT_EQ(centerControl(boardFromFen("8/8/8/3n4/3N4/8/8/8 w - - 0 1")), 0);  // symmetric
  EXPECT_EQ(centerControl(Board::startingPosition()), 0);
}

TEST(Mobility, FavorsTheMoreActiveSide) {
  // White knight in the center, Black has no pieces to move.
  EXPECT_GT(mobilityBalance(boardFromFen("8/8/8/8/3N4/8/8/8 w - - 0 1")), 0);
  // Mirrored knights (d4 vs d5): equal mobility.
  EXPECT_EQ(mobilityBalance(boardFromFen("8/8/8/3n4/3N4/8/8/8 w - - 0 1")), 0);
}

TEST(Evaluate, StartingPositionIsBalanced) {
  EXPECT_EQ(evaluate(Board::startingPosition()), 0);
}

TEST(Evaluate, SymmetricPositionIsBalanced) {
  // Both sides have developed the same knights: no color should be favored.
  EXPECT_EQ(
      evaluate(boardFromFen(
          "r1bqkb1r/pppppppp/2n2n2/8/8/2N2N2/PPPPPPPP/R1BQKB1R w KQkq - 0 1")),
      0);
}

TEST(Evaluate, FavorsStrongerPosition) {
  EXPECT_GT(evaluate(boardFromFen("7k/8/8/8/8/8/8/Q6K w - - 0 1")), 0);
  EXPECT_LT(evaluate(boardFromFen("r6k/8/8/8/8/8/8/7K w - - 0 1")), 0);
}

}  // namespace
}  // namespace stockfih
