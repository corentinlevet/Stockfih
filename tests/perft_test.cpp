#include <gtest/gtest.h>

#include "stockfih/fen.hpp"
#include "stockfih/gamestate.hpp"
#include "stockfih/rules.hpp"

namespace stockfih {
namespace {

// perft: the number of leaf nodes reachable by legal moves to a given depth.
// Matching the well-known reference counts validates move generation, the
// special rules (castling, en passant, promotion), and legality filtering all
// at once. https://www.chessprogramming.org/Perft_Results
long long perft(const Board& board, int depth) {
  if (depth == 0) return 1;
  long long nodes = 0;
  for (const Move& move : generateLegalMoves(board)) {
    nodes += perft(makeMove(board, move), depth - 1);
  }
  return nodes;
}

Board boardFromFen(std::string_view fen) {
  const std::optional<Board> board = parseFen(fen);
  EXPECT_TRUE(board.has_value()) << "bad FEN: " << fen;
  return board.value_or(Board{});
}

TEST(Perft, StartingPosition) {
  const Board board = Board::startingPosition();
  EXPECT_EQ(perft(board, 1), 20);
  EXPECT_EQ(perft(board, 2), 400);
  EXPECT_EQ(perft(board, 3), 8902);
  EXPECT_EQ(perft(board, 4), 197281);
}

TEST(Perft, Kiwipete) {
  // Rich tactical position exercising castling, pins, and captures.
  const Board board = boardFromFen(
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  EXPECT_EQ(perft(board, 1), 48);
  EXPECT_EQ(perft(board, 2), 2039);
  EXPECT_EQ(perft(board, 3), 97862);
}

TEST(Perft, EnPassantHeavyPosition) {
  // Position 3 from the reference table; stresses en passant and checks.
  const Board board = boardFromFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
  EXPECT_EQ(perft(board, 1), 14);
  EXPECT_EQ(perft(board, 2), 191);
  EXPECT_EQ(perft(board, 3), 2812);
  EXPECT_EQ(perft(board, 4), 43238);
}

TEST(Perft, PromotionHeavyPosition) {
  // Position 5 from the reference table; stresses promotions.
  const Board board =
      boardFromFen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  EXPECT_EQ(perft(board, 1), 44);
  EXPECT_EQ(perft(board, 2), 1486);
  EXPECT_EQ(perft(board, 3), 62379);
}

}  // namespace
}  // namespace stockfih
