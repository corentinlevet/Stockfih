#include <gtest/gtest.h>

#include "stockfih/fen.hpp"
#include "stockfih/game.hpp"

namespace stockfih {
namespace {

constexpr Square kE2 = makeSquare(4, 1);
constexpr Square kE4 = makeSquare(4, 3);
constexpr Square kE5 = makeSquare(4, 4);

TEST(Game, LegalMovesFromPawn) {
  const Game game;
  EXPECT_EQ(game.legalMovesFrom(kE2).size(), 2u);  // e3 and e4
}

TEST(Game, RejectsIllegalMove) {
  Game game;
  EXPECT_FALSE(game.playMove(kE2, kE5));  // pawn cannot jump three
  EXPECT_EQ(game.board().sideToMove(), Color::White);
  EXPECT_EQ(game.board().at(kE2), (Piece{PieceType::Pawn, Color::White}));
}

TEST(Game, PlaysLegalMoveAndSwitchesSide) {
  Game game;
  EXPECT_TRUE(game.playMove(kE2, kE4));
  EXPECT_TRUE(game.board().at(kE2).isNone());
  EXPECT_EQ(game.board().at(kE4), (Piece{PieceType::Pawn, Color::White}));
  EXPECT_EQ(game.board().sideToMove(), Color::Black);
}

TEST(Game, AutoPromotesToQueen) {
  Game game(parseFen("k7/4P3/8/8/8/8/8/7K w - - 0 1").value());
  EXPECT_TRUE(game.playMove(makeSquare(4, 6), makeSquare(4, 7)));  // e7-e8
  EXPECT_EQ(game.board().at(makeSquare(4, 7)), (Piece{PieceType::Queen, Color::White}));
}

TEST(Game, BotPlaysALegalMove) {
  Game game;
  const Move move = game.playBestMove(2);
  EXPECT_NE(move.from, move.to);
  EXPECT_EQ(game.board().sideToMove(), Color::Black);
}

TEST(Game, FullGameCanReachCheckmate) {
  // Fool's mate: 1. f3 e5 2. g4 Qh4#.
  Game game;
  EXPECT_TRUE(game.playMove(makeSquare(5, 1), makeSquare(5, 2)));  // f2-f3
  EXPECT_TRUE(game.playMove(makeSquare(4, 6), makeSquare(4, 4)));  // e7-e5
  EXPECT_TRUE(game.playMove(makeSquare(6, 1), makeSquare(6, 3)));  // g2-g4
  EXPECT_TRUE(game.playMove(makeSquare(3, 7), makeSquare(7, 3)));  // Qd8-h4#
  EXPECT_TRUE(game.isCheckmate());
  EXPECT_TRUE(game.isOver());
}

}  // namespace
}  // namespace stockfih
