#include "stockfih/board.hpp"

namespace stockfih {

void Board::clear() { squares_.fill(kNoPiece); }

Board Board::startingPosition() {
  Board board;
  board.clear();

  // Back-rank piece layout from the a-file to the h-file.
  constexpr PieceType kBackRank[kBoardSize] = {
      PieceType::Rook,  PieceType::Knight, PieceType::Bishop, PieceType::Queen,
      PieceType::King,  PieceType::Bishop, PieceType::Knight, PieceType::Rook};

  for (int file = 0; file < kBoardSize; ++file) {
    board.set(makeSquare(file, 0), Piece{kBackRank[file], Color::White});
    board.set(makeSquare(file, 1), Piece{PieceType::Pawn, Color::White});
    board.set(makeSquare(file, 6), Piece{PieceType::Pawn, Color::Black});
    board.set(makeSquare(file, 7), Piece{kBackRank[file], Color::Black});
  }

  board.setSideToMove(Color::White);
  board.setCastlingRights(kAllCastling);
  board.setEnPassantSquare(kNoSquare);
  board.setHalfmoveClock(0);
  board.setFullmoveNumber(1);
  return board;
}

}  // namespace stockfih
