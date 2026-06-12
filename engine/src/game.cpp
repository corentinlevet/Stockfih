#include "stockfih/game.hpp"

#include "stockfih/gamestate.hpp"
#include "stockfih/rules.hpp"
#include "stockfih/search.hpp"

namespace stockfih {

Game::Game() : board_(Board::startingPosition()) {}

Game::Game(const Board& board) : board_(board) {}

std::vector<Move> Game::legalMovesFrom(Square from) const {
  std::vector<Move> result;
  for (const Move& move : generateLegalMoves(board_)) {
    if (move.from == from) result.push_back(move);
  }
  return result;
}

bool Game::playMove(Square from, Square to, PieceType promotion) {
  const std::vector<Move> legal = generateLegalMoves(board_);

  const Move* chosen = nullptr;
  for (const Move& move : legal) {
    if (move.from != from || move.to != to) continue;
    // For a promotion there are four candidates; pick the requested piece.
    if (move.promotion == PieceType::None || move.promotion == promotion) {
      chosen = &move;
      break;
    }
  }

  if (chosen == nullptr) return false;
  board_ = makeMove(board_, *chosen);
  return true;
}

Move Game::playBestMove(int depth) {
  const Move best = findBestMove(board_, depth);
  if (best.from == best.to) return Move{};  // no legal move
  board_ = makeMove(board_, best);
  return best;
}

bool Game::isCheckmate() const { return stockfih::isCheckmate(board_); }

bool Game::isStalemate() const { return stockfih::isStalemate(board_); }

}  // namespace stockfih
