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

bool containsMove(const std::vector<Move>& moves, Square from, Square to) {
  return std::any_of(moves.begin(), moves.end(), [&](const Move& move) {
    return move.from == from && move.to == to;
  });
}

TEST(EnPassant, GeneratedWhenTargetSquareSet) {
  // White pawn e5, black pawn d5, en-passant square d6.
  const Board board = boardFromFen("8/8/8/3pP3/8/8/8/8 w - d6 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_TRUE(containsMove(moves, makeSquare(4, 4), makeSquare(3, 5)));  // exd6
}

TEST(EnPassant, NotGeneratedWithoutTargetSquare) {
  const Board board = boardFromFen("8/8/8/3pP3/8/8/8/8 w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_FALSE(containsMove(moves, makeSquare(4, 4), makeSquare(3, 5)));
}

TEST(EnPassant, MakeMoveRemovesCapturedPawn) {
  const Board board = boardFromFen("8/8/8/3pP3/8/8/8/8 w - d6 0 1");
  const Board after = makeMove(board, Move{makeSquare(4, 4), makeSquare(3, 5)});
  EXPECT_EQ(after.at(makeSquare(3, 5)), (Piece{PieceType::Pawn, Color::White}));  // d6
  EXPECT_TRUE(after.at(makeSquare(3, 4)).isNone());  // d5 captured pawn gone
  EXPECT_TRUE(after.at(makeSquare(4, 4)).isNone());  // e5 vacated
}

TEST(Castling, KingSideGenerated) {
  const Board board = boardFromFen("8/8/8/8/8/8/8/4K2R w K - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_TRUE(containsMove(moves, makeSquare(4, 0), makeSquare(6, 0)));  // O-O
}

TEST(Castling, QueenSideGenerated) {
  const Board board = boardFromFen("8/8/8/8/8/8/8/R3K3 w Q - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_TRUE(containsMove(moves, makeSquare(4, 0), makeSquare(2, 0)));  // O-O-O
}

TEST(Castling, BlockedPathNotGenerated) {
  const Board board = boardFromFen("8/8/8/8/8/8/8/4KB1R w K - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_FALSE(containsMove(moves, makeSquare(4, 0), makeSquare(6, 0)));
}

TEST(Castling, NoRightNotGenerated) {
  const Board board = boardFromFen("8/8/8/8/8/8/8/4K2R w - - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_FALSE(containsMove(moves, makeSquare(4, 0), makeSquare(6, 0)));
}

TEST(Castling, BlackKingSideGenerated) {
  const Board board = boardFromFen("4k2r/8/8/8/8/8/8/8 b k - 0 1");
  const std::vector<Move> moves = generatePseudoLegalMoves(board);
  EXPECT_TRUE(containsMove(moves, makeSquare(4, 7), makeSquare(6, 7)));  // ...O-O
}

TEST(Castling, MakeMoveRelocatesRookAndClearsRights) {
  const Board board = boardFromFen("8/8/8/8/8/8/8/4K2R w K - 0 1");
  const Board after = makeMove(board, Move{makeSquare(4, 0), makeSquare(6, 0)});
  EXPECT_EQ(after.at(makeSquare(6, 0)), (Piece{PieceType::King, Color::White}));  // g1
  EXPECT_EQ(after.at(makeSquare(5, 0)), (Piece{PieceType::Rook, Color::White}));  // f1
  EXPECT_TRUE(after.at(makeSquare(7, 0)).isNone());  // h1 rook moved
  EXPECT_EQ(after.castlingRights(), kNoCastling);
}

TEST(Castling, CapturingRookClearsThatRight) {
  // Black bishop b2 captures the white rook on a1, removing White's queenside right.
  const Board board = boardFromFen("4k3/8/8/8/8/8/1b6/R3K2R b KQ - 0 1");
  const Board after = makeMove(board, Move{makeSquare(1, 1), makeSquare(0, 0)});  // Bxa1
  EXPECT_EQ(after.castlingRights() & kWhiteQueenSide, 0);
  EXPECT_EQ(after.castlingRights() & kWhiteKingSide, kWhiteKingSide);
}

TEST(Promotion, UnderPromotionToKnight) {
  const Board board = boardFromFen("8/4P3/8/8/8/8/8/8 w - - 0 1");
  const Board after =
      makeMove(board, Move{makeSquare(4, 6), makeSquare(4, 7), PieceType::Knight});
  EXPECT_EQ(after.at(makeSquare(4, 7)), (Piece{PieceType::Knight, Color::White}));
}

TEST(EnPassant, TargetClearsAfterAnotherMove) {
  // The en-passant chance must be taken immediately; any other move forfeits it.
  const Board board = boardFromFen("8/8/8/3pP3/8/8/8/8 w - d6 0 1");
  const Board after = makeMove(board, Move{makeSquare(4, 4), makeSquare(4, 5)});  // e5-e6
  EXPECT_EQ(after.enPassantSquare(), kNoSquare);
}

}  // namespace
}  // namespace stockfih
