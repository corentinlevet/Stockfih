#include <gtest/gtest.h>

#include <algorithm>

#include "stockfih/fen.hpp"
#include "stockfih/gamestate.hpp"
#include "stockfih/rules.hpp"
#include "stockfih/search.hpp"

namespace stockfih {
namespace {

Board boardFromFen(std::string_view fen) {
  const std::optional<Board> board = parseFen(fen);
  EXPECT_TRUE(board.has_value()) << "bad FEN: " << fen;
  return board.value_or(Board{});
}

bool isLegal(const Board& board, const Move& move) {
  const std::vector<Move> legal = generateLegalMoves(board);
  return std::any_of(legal.begin(), legal.end(),
                     [&](const Move& m) { return m == move; });
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

TEST(FindBestMove, ReturnsLegalMoveFromStart) {
  const Board board = Board::startingPosition();
  const Move best = findBestMove(board, 2);
  EXPECT_TRUE(isLegal(board, best));
}

TEST(FindBestMove, CapturesFreeQueen) {
  // White rook on a1 can capture the undefended black queen on a8.
  const Board board = boardFromFen("q3k3/8/8/8/8/8/8/R6K w - - 0 1");
  const Move best = findBestMove(board, 2);
  EXPECT_EQ(best.from, makeSquare(0, 0));  // a1
  EXPECT_EQ(best.to, makeSquare(0, 7));    // xa8
}

TEST(FindBestMove, FindsMateInOne) {
  // White mates with Ra1-a8#; the black king is boxed in by its own pawns.
  const Board board = boardFromFen("6k1/5ppp/8/8/8/8/8/R5K1 w - - 0 1");
  const Move best = findBestMove(board, 2);
  const Board after = makeMove(board, best);
  EXPECT_TRUE(isCheckmate(after));
}

TEST(AlphaBeta, MatchesMinimaxScore) {
  const char* positions[] = {
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
      "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1",
      "q3k3/8/8/8/8/8/8/R6K w - - 0 1",
  };
  for (const char* fen : positions) {
    const Board board = boardFromFen(fen);
    for (int depth = 1; depth <= 3; ++depth) {
      EXPECT_EQ(minimax(board, depth), alphaBeta(board, depth))
          << fen << " depth " << depth;
    }
  }
}

TEST(AlphaBeta, VisitsFewerNodesThanMinimax) {
  const Board board = boardFromFen(
      "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1");
  const SearchStats plain = searchWithStats(board, 4, /*useAlphaBeta=*/false);
  const SearchStats pruned = searchWithStats(board, 4, /*useAlphaBeta=*/true);
  EXPECT_EQ(plain.score, pruned.score);
  EXPECT_LT(pruned.nodes, plain.nodes);
}

}  // namespace
}  // namespace stockfih
