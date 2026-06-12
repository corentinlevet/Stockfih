#include <gtest/gtest.h>

#include "stockfih/fen.hpp"
#include "stockfih/gamestate.hpp"

namespace stockfih {
namespace {

Board boardFromFen(std::string_view fen) {
  const std::optional<Board> board = parseFen(fen);
  EXPECT_TRUE(board.has_value()) << "bad FEN: " << fen;
  return board.value_or(Board{});
}

TEST(MakeMove, QuietMoveUpdatesSideAndClock) {
  const Board start = Board::startingPosition();
  const Board after = makeMove(start, Move{makeSquare(6, 0), makeSquare(5, 2)});  // Ng1-f3
  EXPECT_TRUE(after.at(makeSquare(6, 0)).isNone());
  EXPECT_EQ(after.at(makeSquare(5, 2)), (Piece{PieceType::Knight, Color::White}));
  EXPECT_EQ(after.sideToMove(), Color::Black);
  EXPECT_EQ(after.halfmoveClock(), 1);
  EXPECT_EQ(after.fullmoveNumber(), 1);
}

TEST(MakeMove, DoublePawnPushSetsEnPassantSquare) {
  const Board after = makeMove(Board::startingPosition(),
                               Move{makeSquare(4, 1), makeSquare(4, 3)});  // e2-e4
  EXPECT_EQ(after.enPassantSquare(), makeSquare(4, 2));  // e3
  EXPECT_EQ(after.halfmoveClock(), 0);
}

TEST(MakeMove, CaptureResetsClockAndIncrementsFullmoveAfterBlack) {
  // Black to move, a capture: ...exd4.
  const Board board = boardFromFen("8/8/8/8/3P4/8/4p3/8 b - - 3 7");
  const Board after = makeMove(board, Move{makeSquare(4, 1), makeSquare(3, 3)});
  EXPECT_EQ(after.at(makeSquare(3, 3)), (Piece{PieceType::Pawn, Color::Black}));
  EXPECT_EQ(after.halfmoveClock(), 0);
  EXPECT_EQ(after.fullmoveNumber(), 8);
  EXPECT_EQ(after.sideToMove(), Color::White);
}

TEST(MakeMove, KingMoveClearsCastlingRights) {
  const Board board = boardFromFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
  const Board after = makeMove(board, Move{makeSquare(4, 0), makeSquare(4, 1)});  // Ke1-e2
  EXPECT_EQ(after.castlingRights(), kBlackKingSide | kBlackQueenSide);
}

TEST(MakeMove, RookMoveClearsThatSideRight) {
  const Board board = boardFromFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
  const Board after = makeMove(board, Move{makeSquare(7, 0), makeSquare(6, 0)});  // Rh1-g1
  EXPECT_EQ(after.castlingRights() & kWhiteKingSide, 0);
  EXPECT_EQ(after.castlingRights() & kWhiteQueenSide, kWhiteQueenSide);
}

}  // namespace
}  // namespace stockfih
