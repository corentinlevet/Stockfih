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
    addMove(moves, from, ahead);
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
    if (!occupant.isNone() && occupant.color != us) addMove(moves, from, target);
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
      default:
        break;
    }
  }

  return moves;
}

}  // namespace stockfih
