#include <cstdlib>
#include <string>

#include "raylib.h"
#include "renderer.hpp"
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

}  // namespace

int main() {
  using stockfih::gui::BoardLayout;

  constexpr int kMargin = 40;
  const BoardLayout layout{kMargin, kMargin, 80};
  const int boardPixels = stockfih::kBoardSize * layout.squareSize;
  const int windowSize = boardPixels + 2 * kMargin;

  const std::string title = "Stockfih " + std::string(stockfih::version());
  InitWindow(windowSize, windowSize, title.c_str());
  SetTargetFPS(60);

  const char* screenshot = std::getenv("STOCKFIH_SCREENSHOT");
  const int frameBudget = autoCloseFrames();
  int frame = 0;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    stockfih::gui::drawBoardSquares(layout);
    stockfih::gui::drawCoordinates(layout);
    EndDrawing();

    ++frame;
    if (screenshot != nullptr && frame == 2) TakeScreenshot(screenshot);
    if (frameBudget >= 0 && frame >= frameBudget) break;
  }

  CloseWindow();
  return 0;
}
