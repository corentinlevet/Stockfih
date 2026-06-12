#include <gtest/gtest.h>

#include "stockfih/board.hpp"

namespace stockfih {
namespace {

TEST(Board, DefaultIsEmpty) {
  Board board;
  board.clear();
  for (Square square = 0; square < kNumSquares; ++square) {
    EXPECT_TRUE(board.at(square).isNone());
  }
}

TEST(Board, SetAndGet) {
  Board board;
  board.clear();
  const Piece rook{PieceType::Rook, Color::White};
  board.set(makeSquare(0, 0), rook);
  EXPECT_EQ(board.at(makeSquare(0, 0)), rook);
  EXPECT_TRUE(board.at(makeSquare(1, 0)).isNone());
}

TEST(Board, StartingPositionBackRanks) {
  const Board board = Board::startingPosition();
  const PieceType expected[kBoardSize] = {
      PieceType::Rook,  PieceType::Knight, PieceType::Bishop, PieceType::Queen,
      PieceType::King,  PieceType::Bishop, PieceType::Knight, PieceType::Rook};

  for (int file = 0; file < kBoardSize; ++file) {
    const Piece white = board.at(makeSquare(file, 0));
    EXPECT_EQ(white.type, expected[file]);
    EXPECT_EQ(white.color, Color::White);

    const Piece black = board.at(makeSquare(file, 7));
    EXPECT_EQ(black.type, expected[file]);
    EXPECT_EQ(black.color, Color::Black);
  }
}

TEST(Board, StartingPositionPawns) {
  const Board board = Board::startingPosition();
  for (int file = 0; file < kBoardSize; ++file) {
    EXPECT_EQ(board.at(makeSquare(file, 1)), (Piece{PieceType::Pawn, Color::White}));
    EXPECT_EQ(board.at(makeSquare(file, 6)), (Piece{PieceType::Pawn, Color::Black}));
  }
}

TEST(Board, StartingPositionEmptyMiddle) {
  const Board board = Board::startingPosition();
  for (int rank = 2; rank <= 5; ++rank) {
    for (int file = 0; file < kBoardSize; ++file) {
      EXPECT_TRUE(board.at(makeSquare(file, rank)).isNone());
    }
  }
}

TEST(Board, StartingPositionState) {
  const Board board = Board::startingPosition();
  EXPECT_EQ(board.sideToMove(), Color::White);
  EXPECT_EQ(board.castlingRights(), kAllCastling);
  EXPECT_EQ(board.enPassantSquare(), kNoSquare);
  EXPECT_EQ(board.halfmoveClock(), 0);
  EXPECT_EQ(board.fullmoveNumber(), 1);
}

}  // namespace
}  // namespace stockfih
