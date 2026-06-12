#include <gtest/gtest.h>

#include "stockfih/fen.hpp"

namespace stockfih {
namespace {

TEST(Fen, ParseStartingPositionMatchesFactory) {
  const std::optional<Board> parsed = parseFen(kStartingFen);
  ASSERT_TRUE(parsed.has_value());

  const Board expected = Board::startingPosition();
  for (Square square = 0; square < kNumSquares; ++square) {
    EXPECT_EQ(parsed->at(square), expected.at(square)) << "square " << square;
  }
  EXPECT_EQ(parsed->sideToMove(), Color::White);
  EXPECT_EQ(parsed->castlingRights(), kAllCastling);
  EXPECT_EQ(parsed->enPassantSquare(), kNoSquare);
  EXPECT_EQ(parsed->halfmoveClock(), 0);
  EXPECT_EQ(parsed->fullmoveNumber(), 1);
}

TEST(Fen, ParseSideAndCounters) {
  const std::optional<Board> board =
      parseFen("8/8/8/8/8/8/8/8 b KQ - 5 12");
  ASSERT_TRUE(board.has_value());
  EXPECT_EQ(board->sideToMove(), Color::Black);
  EXPECT_EQ(board->castlingRights(), kWhiteKingSide | kWhiteQueenSide);
  EXPECT_EQ(board->halfmoveClock(), 5);
  EXPECT_EQ(board->fullmoveNumber(), 12);
}

TEST(Fen, ParseEnPassantSquare) {
  const std::optional<Board> board =
      parseFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
  ASSERT_TRUE(board.has_value());
  EXPECT_EQ(board->enPassantSquare(), makeSquare(2, 5));  // c6
}

TEST(Fen, ParseNoCastling) {
  const std::optional<Board> board = parseFen("8/8/8/8/8/8/8/8 w - - 0 1");
  ASSERT_TRUE(board.has_value());
  EXPECT_EQ(board->castlingRights(), kNoCastling);
}

TEST(Fen, SerializeStartingPosition) {
  EXPECT_EQ(toFen(Board::startingPosition()), std::string(kStartingFen));
}

TEST(Fen, RoundTrip) {
  const std::string positions[] = {
      std::string(kStartingFen),
      "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2",
      "r3k2r/8/8/8/8/8/8/R3K2R b Kq - 7 25",
      "8/8/8/8/8/8/8/8 w - - 0 1",
  };
  for (const std::string& fen : positions) {
    const std::optional<Board> board = parseFen(fen);
    ASSERT_TRUE(board.has_value()) << fen;
    EXPECT_EQ(toFen(*board), fen);
  }
}

TEST(Fen, RejectsMalformed) {
  EXPECT_FALSE(parseFen("").has_value());
  EXPECT_FALSE(parseFen("8/8/8/8/8/8/8/8 w KQkq - 0").has_value());        // 5 fields
  EXPECT_FALSE(parseFen("8/8/8/8/8/8/8 w - - 0 1").has_value());          // 7 ranks
  EXPECT_FALSE(parseFen("9/8/8/8/8/8/8/8 w - - 0 1").has_value());        // bad count
  EXPECT_FALSE(parseFen("8/8/8/8/8/8/8/8 x - - 0 1").has_value());        // bad color
  EXPECT_FALSE(parseFen("8/8/8/8/8/8/8/8 w - - a 1").has_value());        // bad clock
}

}  // namespace
}  // namespace stockfih
