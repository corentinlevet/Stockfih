#include <gtest/gtest.h>

#include "stockfih/fen.hpp"
#include "stockfih/rules.hpp"

namespace stockfih {
namespace {

Board boardFromFen(std::string_view fen) {
  const std::optional<Board> board = parseFen(fen);
  EXPECT_TRUE(board.has_value()) << "bad FEN: " << fen;
  return board.value_or(Board{});
}

TEST(Attack, RookAttacksAlongRankAndFile) {
  const Board board = boardFromFen("8/8/8/8/3R4/8/8/8 w - - 0 1");  // rook d4
  EXPECT_TRUE(isSquareAttacked(board, makeSquare(3, 0), Color::White));   // d1
  EXPECT_TRUE(isSquareAttacked(board, makeSquare(0, 3), Color::White));   // a4
  EXPECT_FALSE(isSquareAttacked(board, makeSquare(4, 5), Color::White));  // e6 (diagonal)
}

TEST(Attack, BlockedRayDoesNotAttack) {
  // Rook d4, friendly pawn d6 blocks the upward ray beyond d6.
  const Board board = boardFromFen("8/8/3P4/8/3R4/8/8/8 w - - 0 1");
  EXPECT_TRUE(isSquareAttacked(board, makeSquare(3, 4), Color::White));   // d5
  EXPECT_FALSE(isSquareAttacked(board, makeSquare(3, 6), Color::White));  // d7 blocked
}

TEST(Attack, PawnAttacksDiagonally) {
  const Board board = boardFromFen("8/8/8/8/8/8/4P3/8 w - - 0 1");  // white pawn e2
  EXPECT_TRUE(isSquareAttacked(board, makeSquare(3, 2), Color::White));   // d3
  EXPECT_TRUE(isSquareAttacked(board, makeSquare(5, 2), Color::White));   // f3
  EXPECT_FALSE(isSquareAttacked(board, makeSquare(4, 2), Color::White));  // e3 (forward)
}

TEST(Attack, KnightAttacks) {
  const Board board = boardFromFen("8/8/8/8/3N4/8/8/8 w - - 0 1");  // knight d4
  EXPECT_TRUE(isSquareAttacked(board, makeSquare(4, 5), Color::White));   // e6
  EXPECT_FALSE(isSquareAttacked(board, makeSquare(4, 4), Color::White));  // e5
}

TEST(Check, KingInCheckFromRook) {
  const Board board = boardFromFen("4k3/8/8/8/8/8/8/4R3 b - - 0 1");  // Re1 vs Ke8
  EXPECT_TRUE(isInCheck(board, Color::Black));
  EXPECT_FALSE(isInCheck(board, Color::White));
}

TEST(Check, KingNotInCheckWhenBlocked) {
  const Board board = boardFromFen("4k3/4p3/8/8/8/8/8/4R3 b - - 0 1");  // pawn blocks
  EXPECT_FALSE(isInCheck(board, Color::Black));
}

}  // namespace
}  // namespace stockfih
