#include <gtest/gtest.h>

#include <algorithm>

#include "stockfih/fen.hpp"
#include "stockfih/movegen.hpp"

namespace stockfih {
namespace {

// Loads a FEN and aborts the test if it is malformed.
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

// Counts how many generated moves originate from a given square.
int countFrom(const std::vector<Move>& moves, Square from) {
  return static_cast<int>(std::count_if(
      moves.begin(), moves.end(), [&](const Move& move) { return move.from == from; }));
}

TEST(PawnMoves, SingleAndDoublePushFromStart) {
  const Board board = boardFromFen("8/8/8/8/8/8/4P3/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  const Square e2 = makeSquare(4, 1);
  EXPECT_TRUE(containsMove(moves, e2, makeSquare(4, 2)));  // e2-e3
  EXPECT_TRUE(containsMove(moves, e2, makeSquare(4, 3)));  // e2-e4
  EXPECT_EQ(countFrom(moves, e2), 2);
}

TEST(PawnMoves, NoDoublePushAwayFromStart) {
  const Board board = boardFromFen("8/8/8/8/8/4P3/8/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  const Square e3 = makeSquare(4, 2);
  EXPECT_TRUE(containsMove(moves, e3, makeSquare(4, 3)));  // e3-e4
  EXPECT_EQ(countFrom(moves, e3), 1);
}

TEST(PawnMoves, BlockedPawnDoesNotMove) {
  const Board board = boardFromFen("8/8/8/8/8/4p3/4P3/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_EQ(countFrom(moves, makeSquare(4, 1)), 0);
}

TEST(PawnMoves, DiagonalCaptures) {
  const Board board = boardFromFen("8/8/8/8/8/3p1p2/4P3/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  const Square e2 = makeSquare(4, 1);
  EXPECT_TRUE(containsMove(moves, e2, makeSquare(3, 2)));  // exd3
  EXPECT_TRUE(containsMove(moves, e2, makeSquare(5, 2)));  // exf3
  EXPECT_EQ(countFrom(moves, e2), 4);  // two pushes + two captures
}

TEST(PawnMoves, BlackPawnMovesDownward) {
  const Board board = boardFromFen("8/4p3/8/8/8/8/8/8 b - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  const Square e7 = makeSquare(4, 6);
  EXPECT_TRUE(containsMove(moves, e7, makeSquare(4, 5)));  // e7-e6
  EXPECT_TRUE(containsMove(moves, e7, makeSquare(4, 4)));  // e7-e5
  EXPECT_EQ(countFrom(moves, e7), 2);
}

TEST(KnightMoves, CenterHasEightMoves) {
  const Board board = boardFromFen("8/8/8/8/3N4/8/8/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_EQ(countFrom(moves, makeSquare(3, 3)), 8);  // d4
}

TEST(KnightMoves, CornerHasTwoMoves) {
  const Board board = boardFromFen("8/8/8/8/8/8/8/N7 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  const Square a1 = makeSquare(0, 0);
  EXPECT_TRUE(containsMove(moves, a1, makeSquare(1, 2)));  // b3
  EXPECT_TRUE(containsMove(moves, a1, makeSquare(2, 1)));  // c2
  EXPECT_EQ(countFrom(moves, a1), 2);
}

TEST(KnightMoves, BlockedByFriendlyButCapturesEnemy) {
  // Knight on d4; friendly pawn on e6 blocks one target, enemy pawn on c6 is a
  // capture target.
  const Board board = boardFromFen("8/8/2p1P3/8/3N4/8/8/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  const Square d4 = makeSquare(3, 3);
  EXPECT_FALSE(containsMove(moves, d4, makeSquare(4, 5)));  // e6 friendly: blocked
  EXPECT_TRUE(containsMove(moves, d4, makeSquare(2, 5)));   // c6 enemy: capture
  EXPECT_EQ(countFrom(moves, d4), 7);
}

TEST(BishopMoves, CenterHasThirteenMoves) {
  const Board board = boardFromFen("8/8/8/8/3B4/8/8/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_EQ(countFrom(moves, makeSquare(3, 3)), 13);  // d4
}

TEST(BishopMoves, StopsAtFriendlyAndCapturesEnemy) {
  // Bishop d4; friendly pawn f6 stops the up-right ray before f6; enemy pawn b2
  // is captured on the down-left ray.
  const Board board = boardFromFen("8/8/5P2/8/3B4/8/1p6/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  const Square d4 = makeSquare(3, 3);
  EXPECT_TRUE(containsMove(moves, d4, makeSquare(4, 4)));   // e5
  EXPECT_FALSE(containsMove(moves, d4, makeSquare(5, 5)));  // f6 friendly: blocked
  EXPECT_TRUE(containsMove(moves, d4, makeSquare(2, 2)));   // c3
  EXPECT_TRUE(containsMove(moves, d4, makeSquare(1, 1)));   // b2 enemy: capture
  EXPECT_FALSE(containsMove(moves, d4, makeSquare(0, 0)));  // a1: beyond capture
}

TEST(RookMoves, CenterHasFourteenMoves) {
  const Board board = boardFromFen("8/8/8/8/3R4/8/8/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_EQ(countFrom(moves, makeSquare(3, 3)), 14);  // d4
}

TEST(RookMoves, StopsAtFriendlyAndCapturesEnemy) {
  // Rook d4; enemy pawn d7 captured upward, friendly pawn a4 blocks left ray.
  const Board board = boardFromFen("8/3p4/8/8/P2R4/8/8/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  const Square d4 = makeSquare(3, 3);
  EXPECT_TRUE(containsMove(moves, d4, makeSquare(3, 6)));   // d7 enemy: capture
  EXPECT_FALSE(containsMove(moves, d4, makeSquare(3, 7)));  // d8: beyond capture
  EXPECT_TRUE(containsMove(moves, d4, makeSquare(2, 3)));   // c4
  EXPECT_FALSE(containsMove(moves, d4, makeSquare(0, 3)));  // a4 friendly: blocked
}

TEST(QueenMoves, CenterHasTwentySevenMoves) {
  const Board board = boardFromFen("8/8/8/8/3Q4/8/8/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_EQ(countFrom(moves, makeSquare(3, 3)), 27);  // d4: 14 rook + 13 bishop
}

}  // namespace
}  // namespace stockfih
