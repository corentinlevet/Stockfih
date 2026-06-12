#include "stockfih/gamestate.hpp"

#include <cstdlib>

namespace stockfih {
namespace {

// Clears the castling right tied to a rook home square whenever a piece leaves
// or is captured there.
void clearRookRight(std::uint8_t& rights, Square square) {
  if (square == makeSquare(0, 0)) rights &= ~kWhiteQueenSide;
  else if (square == makeSquare(7, 0)) rights &= ~kWhiteKingSide;
  else if (square == makeSquare(0, 7)) rights &= ~kBlackQueenSide;
  else if (square == makeSquare(7, 7)) rights &= ~kBlackKingSide;
}

}  // namespace

Board makeMove(const Board& board, const Move& move) {
  Board next = board;
  const Color us = board.sideToMove();
  const Piece moving = board.at(move.from);
  const bool isPawn = moving.type == PieceType::Pawn;
  const bool isCapture = !board.at(move.to).isNone();

  next.set(move.from, kNoPiece);

  // En passant: a pawn moving diagonally onto the empty en-passant square.
  const bool enPassant = isPawn && move.to == board.enPassantSquare() &&
                         board.enPassantSquare() != kNoSquare &&
                         fileOf(move.from) != fileOf(move.to);
  if (enPassant) {
    next.set(makeSquare(fileOf(move.to), rankOf(move.from)), kNoPiece);
  }

  // Place the moving piece, applying promotion if requested.
  if (move.promotion != PieceType::None) {
    next.set(move.to, Piece{move.promotion, us});
  } else {
    next.set(move.to, moving);
  }

  // Castling: the king stepped two files, so relocate the corresponding rook.
  if (moving.type == PieceType::King &&
      std::abs(fileOf(move.to) - fileOf(move.from)) == 2) {
    const int rank = rankOf(move.from);
    if (fileOf(move.to) > fileOf(move.from)) {  // king side
      next.set(makeSquare(7, rank), kNoPiece);
      next.set(makeSquare(5, rank), Piece{PieceType::Rook, us});
    } else {  // queen side
      next.set(makeSquare(0, rank), kNoPiece);
      next.set(makeSquare(3, rank), Piece{PieceType::Rook, us});
    }
  }

  // Castling rights: lose both on a king move, lose one when a rook leaves or
  // is captured on its home square.
  std::uint8_t rights = board.castlingRights();
  if (moving.type == PieceType::King) {
    rights &= us == Color::White ? ~(kWhiteKingSide | kWhiteQueenSide)
                                 : ~(kBlackKingSide | kBlackQueenSide);
  }
  clearRookRight(rights, move.from);
  clearRookRight(rights, move.to);
  next.setCastlingRights(rights);

  // En-passant target square: set behind a double pawn push, cleared otherwise.
  if (isPawn && std::abs(rankOf(move.to) - rankOf(move.from)) == 2) {
    next.setEnPassantSquare(
        makeSquare(fileOf(move.from), (rankOf(move.from) + rankOf(move.to)) / 2));
  } else {
    next.setEnPassantSquare(kNoSquare);
  }

  // Move counters.
  if (isPawn || isCapture || enPassant) {
    next.setHalfmoveClock(0);
  } else {
    next.setHalfmoveClock(board.halfmoveClock() + 1);
  }
  if (us == Color::Black) next.setFullmoveNumber(board.fullmoveNumber() + 1);

  next.setSideToMove(opposite(us));
  return next;
}

}  // namespace stockfih
