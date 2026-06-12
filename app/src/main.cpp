#include <cstdlib>
#include <string>

#include "raylib.h"
#include "stockfih/version.hpp"

namespace {

// When STOCKFIH_AUTOCLOSE is set, the window closes after that many frames.
// This lets the GUI be smoke-tested without a human closing the window.
int autoCloseFrames() {
  if (const char* value = std::getenv("STOCKFIH_AUTOCLOSE")) {
    return std::atoi(value);
  }
  return -1;  // disabled: run until the user closes the window
}

}  // namespace

int main() {
  constexpr int kScreenWidth = 800;
  constexpr int kScreenHeight = 600;

  const std::string title = "Stockfih " + std::string(stockfih::version());
  InitWindow(kScreenWidth, kScreenHeight, title.c_str());
  SetTargetFPS(60);

  const int frameBudget = autoCloseFrames();
  int frame = 0;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(title.c_str(), 20, 20, 30, DARKGRAY);
    DrawText("raylib window is up", 20, 70, 20, GRAY);
    EndDrawing();

    if (frameBudget >= 0 && ++frame >= frameBudget) {
      break;
    }
  }

  CloseWindow();
  return 0;
}
