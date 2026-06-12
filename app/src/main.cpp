#include <cstdlib>
#include <string>
#include <vector>

#include "input.hpp"
#include "raylib.h"
#include "renderer.hpp"
#include "stockfih/game.hpp"
#include "stockfih/square.hpp"
#include "stockfih/version.hpp"

namespace {

// When STOCKFIH_AUTOCLOSE is set, the window closes after that many frames so
// the GUI can be smoke-tested without a human closing the window.
int autoCloseFrames() {
  if (const char* value = std::getenv("STOCKFIH_AUTOCLOSE")) {
    return std::atoi(value);
  }
  return -1;
}

const char* statusText(const stockfih::Game& game) {
  if (game.isCheckmate()) {
    // The side to move has been mated, so the other side won.
    return game.board().sideToMove() == stockfih::Color::White ? "Checkmate - Black wins"
                                                               : "Checkmate - White wins";
  }
  if (game.isStalemate()) return "Stalemate - draw";
  return game.board().sideToMove() == stockfih::Color::White ? "Your move (White)"
                                                             : "Stockfih is thinking...";
}

}  // namespace

int main() {
  using stockfih::Move;
  using stockfih::Square;
  using stockfih::gui::BoardLayout;

  constexpr int kMargin = 40;
  constexpr int kBotDepth = 3;
  constexpr stockfih::Color kHuman = stockfih::Color::White;

  const BoardLayout layout{kMargin, kMargin, 80};
  const int boardPixels = stockfih::kBoardSize * layout.squareSize;
  const int windowSize = boardPixels + 2 * kMargin;

  const std::string title = "Stockfih " + std::string(stockfih::version());
  InitWindow(windowSize, windowSize, title.c_str());
  SetTargetFPS(60);

  stockfih::Game game;
  Square selected = stockfih::kNoSquare;
  std::vector<Move> selectedMoves;

  // Optional demo selection so a headless screenshot can show the highlights.
  if (std::getenv("STOCKFIH_SCREENSHOT") != nullptr) {
    selected = stockfih::makeSquare(4, 1);  // e2
    selectedMoves = game.legalMovesFrom(selected);
  }

  const char* screenshot = std::getenv("STOCKFIH_SCREENSHOT");
  const int frameBudget = autoCloseFrames();
  int frame = 0;

  while (!WindowShouldClose()) {
    // Bot reply when it is its turn.
    if (!game.isOver() && game.board().sideToMove() != kHuman) {
      game.playBestMove(kBotDepth);
      selected = stockfih::kNoSquare;
      selectedMoves.clear();
    }

    // Human input.
    if (!game.isOver() && game.board().sideToMove() == kHuman &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      const Square clicked =
          stockfih::gui::squareAtPixel(layout, GetMousePosition());
      if (stockfih::isValidSquare(clicked)) {
        bool moved = false;
        for (const Move& move : selectedMoves) {
          if (move.to == clicked) {
            game.playMove(selected, clicked);
            moved = true;
            break;
          }
        }
        if (moved) {
          selected = stockfih::kNoSquare;
          selectedMoves.clear();
        } else {
          const stockfih::Piece piece = game.board().at(clicked);
          if (!piece.isNone() && piece.color == kHuman) {
            selected = clicked;
            selectedMoves = game.legalMovesFrom(clicked);
          } else {
            selected = stockfih::kNoSquare;
            selectedMoves.clear();
          }
        }
      }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    stockfih::gui::drawBoardSquares(layout);
    stockfih::gui::drawHighlights(layout, selected, selectedMoves);
    stockfih::gui::drawCoordinates(layout);
    stockfih::gui::drawPieces(layout, game.board());
    stockfih::gui::drawStatus(layout, statusText(game));
    EndDrawing();

    ++frame;
    if (screenshot != nullptr && frame == 2) TakeScreenshot(screenshot);
    if (frameBudget >= 0 && frame >= frameBudget) break;
  }

  CloseWindow();
  return 0;
}
