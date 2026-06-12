#include "stockfih/rules.hpp"

#include <cstddef>
#include <cstdlib>

#include "stockfih/gamestate.hpp"
#include "stockfih/movegen.hpp"

namespace stockfih {
namespace {

struct Offset {
  int file;
  int rank;
};

constexpr Offset kKnightOffsets[] = {{1, 2},  {2, 1},   {2, -1},  {1, -2},
                                     {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};
constexpr Offset kKingOffsets[] = {{1, 0},  {1, 1},   {0, 1},  {-1, 1},
                                   {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
constexpr Offset kBishopDirections[] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
constexpr Offset kRookDirections[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

bool isAttackerOfType(Piece piece, Color by, PieceType type) {
  return !piece.isNone() && piece.color == by && piece.type == type;
}

template <std::size_t N>
bool slidingAttack(const Board& board, int file, int rank, Color by,
                   const Offset (&directions)[N], PieceType slider) {
  for (const Offset& direction : directions) {
    int targetFile = file + direction.file;
    int targetRank = rank + direction.rank;
    while (isOnBoard(targetFile, targetRank)) {
      const Piece piece = board.at(makeSquare(targetFile, targetRank));
      if (!piece.isNone()) {
        if (piece.color == by &&
            (piece.type == slider || piece.type == PieceType::Queen)) {
          return true;
        }
        break;  // any other piece blocks the ray
      }
      targetFile += direction.file;
      targetRank += direction.rank;
    }
  }
  return false;
}

}  // namespace

bool isSquareAttacked(const Board& board, Square square, Color byColor) {
  const int file = fileOf(square);
  const int rank = rankOf(square);

  // Pawn attacks: a `byColor` pawn sits one rank back along its march direction.
  const int pawnRank = byColor == Color::White ? rank - 1 : rank + 1;
  for (const int sideStep : {-1, 1}) {
    if (isOnBoard(file + sideStep, pawnRank) &&
        isAttackerOfType(board.at(makeSquare(file + sideStep, pawnRank)), byColor,
                         PieceType::Pawn)) {
      return true;
    }
  }

  for (const Offset& offset : kKnightOffsets) {
    if (isOnBoard(file + offset.file, rank + offset.rank) &&
        isAttackerOfType(board.at(makeSquare(file + offset.file, rank + offset.rank)),
                         byColor, PieceType::Knight)) {
      return true;
    }
  }

  for (const Offset& offset : kKingOffsets) {
    if (isOnBoard(file + offset.file, rank + offset.rank) &&
        isAttackerOfType(board.at(makeSquare(file + offset.file, rank + offset.rank)),
                         byColor, PieceType::King)) {
      return true;
    }
  }

  if (slidingAttack(board, file, rank, byColor, kBishopDirections, PieceType::Bishop)) {
    return true;
  }
  if (slidingAttack(board, file, rank, byColor, kRookDirections, PieceType::Rook)) {
    return true;
  }
  return false;
}

Square findKing(const Board& board, Color color) {
  for (Square square = 0; square < kNumSquares; ++square) {
    const Piece piece = board.at(square);
    if (piece.type == PieceType::King && piece.color == color) return square;
  }
  return kNoSquare;
}

bool isInCheck(const Board& board, Color color) {
  const Square king = findKing(board, color);
  if (king == kNoSquare) return false;
  return isSquareAttacked(board, king, opposite(color));
}

std::vector<Move> generateLegalMoves(const Board& board) {
  const Color us = board.sideToMove();
  std::vector<Move> legal;

  for (const Move& move : generatePseudoLegalMoves(board)) {
    const Piece moving = board.at(move.from);

    // Castling may not start in, pass through, or end in check. The starting
    // and transit squares are checked here; the destination is covered by the
    // post-move check below.
    if (moving.type == PieceType::King &&
        std::abs(fileOf(move.to) - fileOf(move.from)) == 2) {
      if (isInCheck(board, us)) continue;
      const int rank = rankOf(move.from);
      const int transitFile = (fileOf(move.from) + fileOf(move.to)) / 2;
      if (isSquareAttacked(board, makeSquare(transitFile, rank), opposite(us))) {
        continue;
      }
    }

    const Board next = makeMove(board, move);
    if (!isInCheck(next, us)) legal.push_back(move);
  }

  return legal;
}

}  // namespace stockfih
