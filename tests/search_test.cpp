#include <gtest/gtest.h>

#include "stockfih/fen.hpp"
#include "stockfih/search.hpp"

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

TEST(Evaluate, ExtraQueenFavorsWhite) {
  const Board board = boardFromFen("7k/8/8/8/8/8/8/Q6K w - - 0 1");
  EXPECT_EQ(evaluate(board), 900);
}

TEST(Evaluate, ExtraRookFavorsBlack) {
  const Board board = boardFromFen("r6k/8/8/8/8/8/8/7K w - - 0 1");
  EXPECT_EQ(evaluate(board), -500);
}

TEST(Minimax, DetectsCheckmateForWhite) {
  // Black is already checkmated (back-rank); White is winning.
  const Board board = boardFromFen("4R1k1/5ppp/8/8/8/8/8/6K1 b - - 0 1");
  EXPECT_EQ(minimax(board, 3), kMateScore);
}

TEST(Minimax, StalemateIsDraw) {
  const Board board = boardFromFen("7k/5Q2/6K1/8/8/8/8/8 b - - 0 1");
  EXPECT_EQ(minimax(board, 3), 0);
}

}  // namespace
}  // namespace stockfih
