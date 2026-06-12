#include <gtest/gtest.h>

#include <algorithm>

#include "stockfih/fen.hpp"
#include "stockfih/gamestate.hpp"
#include "stockfih/movegen.hpp"

namespace stockfih {
namespace {

Board boardFromFen(std::string_view fen) {
  const std::optional<Board> board = parseFen(fen);
  EXPECT_TRUE(board.has_value()) << "bad FEN: " << fen;
  return board.value_or(Board{});
}

bool containsPromotion(const std::vector<Move>& moves, Square from, Square to,
                       PieceType promotion) {
  return std::any_of(moves.begin(), moves.end(), [&](const Move& move) {
    return move.from == from && move.to == to && move.promotion == promotion;
  });
}

int countTo(const std::vector<Move>& moves, Square to) {
  return static_cast<int>(std::count_if(
      moves.begin(), moves.end(), [&](const Move& move) { return move.to == to; }));
}

TEST(Promotion, PushGeneratesFourChoices) {
  const Board board = boardFromFen("8/4P3/8/8/8/8/8/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  const Square e7 = makeSquare(4, 6);
  const Square e8 = makeSquare(4, 7);
  EXPECT_EQ(countTo(moves, e8), 4);
  EXPECT_TRUE(containsPromotion(moves, e7, e8, PieceType::Queen));
  EXPECT_TRUE(containsPromotion(moves, e7, e8, PieceType::Rook));
  EXPECT_TRUE(containsPromotion(moves, e7, e8, PieceType::Bishop));
  EXPECT_TRUE(containsPromotion(moves, e7, e8, PieceType::Knight));
}

TEST(Promotion, CaptureGeneratesFourChoices) {
  const Board board = boardFromFen("5r2/4P3/8/8/8/8/8/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  const Square f8 = makeSquare(5, 7);
  EXPECT_EQ(countTo(moves, f8), 4);  // exf8=Q/R/B/N
}

TEST(Promotion, MakeMoveReplacesPawn) {
  const Board board = boardFromFen("8/4P3/8/8/8/8/8/8 w - - 0 1");
  const Board after =
      makeMove(board, Move{makeSquare(4, 6), makeSquare(4, 7), PieceType::Queen});
  EXPECT_EQ(after.at(makeSquare(4, 7)), (Piece{PieceType::Queen, Color::White}));
  EXPECT_TRUE(after.at(makeSquare(4, 6)).isNone());
}

}  // namespace
}  // namespace stockfih
