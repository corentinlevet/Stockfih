#include <gtest/gtest.h>

#include <algorithm>

#include "stockfih/fen.hpp"
#include "stockfih/gamestate.hpp"
#include "stockfih/rules.hpp"

namespace stockfih {
namespace {

Board boardFromFen(std::string_view fen) {
  const std::optional<Board> board = parseFen(fen);
  EXPECT_TRUE(board.has_value()) << "bad FEN: " << fen;
  return board.value_or(Board{});
}

bool containsMove(const std::vector<Move>& moves, Square from, Square to) {
  return std::any_of(moves.begin(), moves.end(), [&](const Move& move) {
    return move.from == from && move.to == to;
  });
}

// Asserts that no legal move leaves the moving side's king in check.
void expectAllMovesLeaveKingSafe(const Board& board) {
  const Color us = board.sideToMove();
  for (const Move& move : generateLegalMoves(board)) {
    EXPECT_FALSE(isInCheck(makeMove(board, move), us));
  }
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

TEST(LegalMoves, StartingPositionHasTwenty) {
  EXPECT_EQ(generateLegalMoves(Board::startingPosition()).size(), 20u);
}

TEST(LegalMoves, PinnedPieceCannotMove) {
  // White knight e2 is pinned to the king e1 by the black rook e8.
  const Board board = boardFromFen("4r3/8/8/8/8/8/4N3/4K3 w - - 0 1");
  const std::vector<Move> legal = generateLegalMoves(board);
  EXPECT_EQ(std::count_if(legal.begin(), legal.end(),
                          [](const Move& m) { return m.from == makeSquare(4, 1); }),
            0);  // the pinned knight has no legal move
  expectAllMovesLeaveKingSafe(board);
}

TEST(LegalMoves, MustEscapeCheck) {
  // Black king e8 in check from the white rook on e1.
  const Board board = boardFromFen("4k3/8/8/8/8/8/8/4R3 b - - 0 1");
  const std::vector<Move> legal = generateLegalMoves(board);
  EXPECT_FALSE(legal.empty());
  // Every legal move is a king move that leaves the e-file.
  for (const Move& move : legal) {
    EXPECT_EQ(move.from, makeSquare(4, 7));
    EXPECT_NE(fileOf(move.to), 4);
  }
  expectAllMovesLeaveKingSafe(board);
}

TEST(LegalMoves, BlockingCheckIsLegal) {
  // Black rook e8 checks the white king e1; white rook a4 can interpose on e4.
  const Board board = boardFromFen("4r3/8/8/8/R7/8/8/4K3 w - - 0 1");
  const std::vector<Move> legal = generateLegalMoves(board);
  EXPECT_TRUE(containsMove(legal, makeSquare(0, 3), makeSquare(4, 3)));  // Ra4-e4 blocks
  expectAllMovesLeaveKingSafe(board);
}

TEST(LegalMoves, CannotCastleThroughCheck) {
  // White could castle king-side, but f1 is attacked by the black rook on f8.
  const Board board = boardFromFen("5r2/8/8/8/8/8/8/4K2R w K - 0 1");
  const std::vector<Move> legal = generateLegalMoves(board);
  EXPECT_FALSE(containsMove(legal, makeSquare(4, 0), makeSquare(6, 0)));
}

TEST(LegalMoves, CanCastleWhenPathSafe) {
  const Board board = boardFromFen("8/8/8/8/8/8/8/4K2R w K - 0 1");
  const std::vector<Move> legal = generateLegalMoves(board);
  EXPECT_TRUE(containsMove(legal, makeSquare(4, 0), makeSquare(6, 0)));
}

}  // namespace
}  // namespace stockfih
