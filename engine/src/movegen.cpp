#include "stockfih/movegen.hpp"

#include <cstddef>

namespace stockfih {
namespace {

struct Offset {
  int file;
  int rank;
};

void addMove(std::vector<Move>& moves, Square from, Square to) {
  moves.push_back(Move{from, to, PieceType::None});
}

bool isPromotionRank(int rank, Color us) {
  return us == Color::White ? rank == kBoardSize - 1 : rank == 0;
}

// Adds a pawn move, expanding it into the four promotion choices when the pawn
// reaches the last rank.
void addPawnMove(std::vector<Move>& moves, Square from, Square to, Color us) {
  if (isPromotionRank(rankOf(to), us)) {
    for (const PieceType promotion : {PieceType::Queen, PieceType::Rook,
                                      PieceType::Bishop, PieceType::Knight}) {
      moves.push_back(Move{from, to, promotion});
    }
  } else {
    addMove(moves, from, to);
  }
}

// Single-step ("jumping") moves: each offset is tried once. A target is legal
// if it is on the board and not occupied by a friendly piece. Used by the
// knight and the king.
template <std::size_t N>
void generateStepMoves(const Board& board, Square from, Color us,
                       const Offset (&offsets)[N], std::vector<Move>& moves) {
  const int file = fileOf(from);
  const int rank = rankOf(from);
  for (const Offset& offset : offsets) {
    const int targetFile = file + offset.file;
    const int targetRank = rank + offset.rank;
    if (!isOnBoard(targetFile, targetRank)) continue;
    const Square target = makeSquare(targetFile, targetRank);
    const Piece occupant = board.at(target);
    if (occupant.isNone() || occupant.color != us) addMove(moves, from, target);
  }
}

void generateKnightMoves(const Board& board, Square from, Color us,
                         std::vector<Move>& moves) {
  static constexpr Offset kKnightOffsets[] = {
      {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};
  generateStepMoves(board, from, us, kKnightOffsets, moves);
}

// Sliding moves: walk along each direction until leaving the board or hitting a
// piece. A friendly piece blocks; an enemy piece is captured and then blocks.
// Used by the bishop, rook, and queen.
template <std::size_t N>
void generateSlidingMoves(const Board& board, Square from, Color us,
                          const Offset (&directions)[N], std::vector<Move>& moves) {
  for (const Offset& direction : directions) {
    int targetFile = fileOf(from) + direction.file;
    int targetRank = rankOf(from) + direction.rank;
    while (isOnBoard(targetFile, targetRank)) {
      const Square target = makeSquare(targetFile, targetRank);
      const Piece occupant = board.at(target);
      if (occupant.isNone()) {
        addMove(moves, from, target);
      } else {
        if (occupant.color != us) addMove(moves, from, target);
        break;
      }
      targetFile += direction.file;
      targetRank += direction.rank;
    }
  }
}

constexpr Offset kBishopDirections[] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

void generateBishopMoves(const Board& board, Square from, Color us,
                         std::vector<Move>& moves) {
  generateSlidingMoves(board, from, us, kBishopDirections, moves);
}

constexpr Offset kRookDirections[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

void generateRookMoves(const Board& board, Square from, Color us,
                       std::vector<Move>& moves) {
  generateSlidingMoves(board, from, us, kRookDirections, moves);
}

void generateQueenMoves(const Board& board, Square from, Color us,
                        std::vector<Move>& moves) {
  generateSlidingMoves(board, from, us, kBishopDirections, moves);
  generateSlidingMoves(board, from, us, kRookDirections, moves);
}

// Adds castling moves when the king is on its home square, the relevant right
// is held, the squares between king and rook are empty, and the rook is home.
// King safety (not castling out of, through, or into check) is enforced later
// by the legality filter in issue #6.
void generateCastlingMoves(const Board& board, Square from, Color us,
                           std::vector<Move>& moves) {
  const int homeRank = us == Color::White ? 0 : 7;
  if (from != makeSquare(4, homeRank)) return;

  const std::uint8_t rights = board.castlingRights();
  const std::uint8_t kingSide = us == Color::White ? kWhiteKingSide : kBlackKingSide;
  const std::uint8_t queenSide = us == Color::White ? kWhiteQueenSide : kBlackQueenSide;
  const Piece ownRook{PieceType::Rook, us};

  if ((rights & kingSide) && board.at(makeSquare(5, homeRank)).isNone() &&
      board.at(makeSquare(6, homeRank)).isNone() &&
      board.at(makeSquare(7, homeRank)) == ownRook) {
    addMove(moves, from, makeSquare(6, homeRank));
  }

  if ((rights & queenSide) && board.at(makeSquare(1, homeRank)).isNone() &&
      board.at(makeSquare(2, homeRank)).isNone() &&
      board.at(makeSquare(3, homeRank)).isNone() &&
      board.at(makeSquare(0, homeRank)) == ownRook) {
    addMove(moves, from, makeSquare(2, homeRank));
  }
}

// The king steps one square in any of the eight directions, plus castling.
void generateKingMoves(const Board& board, Square from, Color us,
                       std::vector<Move>& moves) {
  static constexpr Offset kKingOffsets[] = {
      {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
  generateStepMoves(board, from, us, kKingOffsets, moves);
  generateCastlingMoves(board, from, us, moves);
}

// Pawns push forward one square (two from their starting rank) and capture
// diagonally forward onto enemy pieces. Promotion and en passant are added in
// issue #5.
void generatePawnMoves(const Board& board, Square from, Color us,
                       std::vector<Move>& moves) {
  const int file = fileOf(from);
  const int rank = rankOf(from);
  const int forward = us == Color::White ? 1 : -1;
  const int startRank = us == Color::White ? 1 : 6;

  const int nextRank = rank + forward;
  if (!isOnBoard(file, nextRank)) return;

  const Square ahead = makeSquare(file, nextRank);
  if (board.at(ahead).isNone()) {
    addPawnMove(moves, from, ahead, us);
    if (rank == startRank) {
      const Square twoAhead = makeSquare(file, rank + 2 * forward);
      if (board.at(twoAhead).isNone()) addMove(moves, from, twoAhead);
    }
  }

  for (const int sideStep : {-1, 1}) {
    const int captureFile = file + sideStep;
    if (!isOnBoard(captureFile, nextRank)) continue;
    const Square target = makeSquare(captureFile, nextRank);
    const Piece occupant = board.at(target);
    if (!occupant.isNone() && occupant.color != us) {
      addPawnMove(moves, from, target, us);
    } else if (occupant.isNone() && target == board.enPassantSquare() &&
               board.enPassantSquare() != kNoSquare) {
      addMove(moves, from, target);  // en passant capture
    }
  }
}

}  // namespace

std::vector<Move> generatePseudoLegalMoves(const Board& board) {
  std::vector<Move> moves;
  const Color us = board.sideToMove();

  for (Square square = 0; square < kNumSquares; ++square) {
    const Piece piece = board.at(square);
    if (piece.isNone() || piece.color != us) continue;

    switch (piece.type) {
      case PieceType::Pawn:
        generatePawnMoves(board, square, us, moves);
        break;
      case PieceType::Knight:
        generateKnightMoves(board, square, us, moves);
        break;
      case PieceType::Bishop:
        generateBishopMoves(board, square, us, moves);
        break;
      case PieceType::Rook:
        generateRookMoves(board, square, us, moves);
        break;
      case PieceType::Queen:
        generateQueenMoves(board, square, us, moves);
        break;
      case PieceType::King:
        generateKingMoves(board, square, us, moves);
        break;
      default:
        break;
    }
  }

  return moves;
}

}  // namespace stockfih
