#pragma once

#include <vector>

#include "stockfih/board.hpp"
#include "stockfih/move.hpp"

namespace stockfih {

// A playable chess game: holds the current position and drives it forward with
// human moves and engine replies. Raylib-free so it can be unit-tested and
// reused by the GUI front-end.
class Game {
 public:
  Game();
  explicit Game(const Board& board);

  [[nodiscard]] const Board& board() const { return board_; }

  // Legal moves whose origin is `from` (for highlighting a selected piece).
  [[nodiscard]] std::vector<Move> legalMovesFrom(Square from) const;

  // Plays the move from->to if it is legal. Promotions use `promotion`
  // (defaults to a queen). Returns true if a move was made.
  bool playMove(Square from, Square to, PieceType promotion = PieceType::Queen);

  // Plays the engine's best move for the side to move, searched to `depth`
  // plies. Returns the move played, or a default Move when none exists.
  Move playBestMove(int depth);

  [[nodiscard]] bool isCheckmate() const;
  [[nodiscard]] bool isStalemate() const;
  [[nodiscard]] bool isOver() const { return isCheckmate() || isStalemate(); }

 private:
  Board board_;
};

}  // namespace stockfih
