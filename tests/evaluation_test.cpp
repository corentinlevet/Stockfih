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

TEST(Evaluate, StartingPositionIsBalanced) {
  EXPECT_EQ(evaluate(Board::startingPosition()), 0);
}

TEST(Evaluate, ExtraMaterialFavorsTheOwner) {
  EXPECT_GT(evaluate(boardFromFen("7k/8/8/8/8/8/8/Q6K w - - 0 1")), 0);
  EXPECT_LT(evaluate(boardFromFen("r6k/8/8/8/8/8/8/7K w - - 0 1")), 0);
}

}  // namespace
}  // namespace stockfih
