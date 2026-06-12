# Stockfih

A chess engine and graphical interface written from scratch in modern C++.

Stockfih pairs a fully headless chess engine (board representation, move
generation, rules, and a Minimax / Alpha-Beta bot) with a [raylib](https://www.raylib.com/)
graphical interface that lets a human play against the bot.

## Architecture

The codebase is split so the engine can be unit-tested without any graphics:

| Target            | Kind         | Depends on        |
| ----------------- | ------------ | ----------------- |
| `stockfih_engine` | static lib   | nothing (headless)|
| `stockfih`        | executable   | engine + raylib   |
| `stockfih_tests`  | test runner  | engine + GoogleTest |

```
engine/   headless chess engine (no raylib)
app/      raylib GUI executable
tests/    GoogleTest unit tests (link engine only)
cmake/    dependency wiring (FetchContent)
```

## Requirements

- CMake >= 3.20
- A C++20 compiler (GCC, Clang, or MSVC)
- Ninja (recommended) or any CMake generator

raylib and GoogleTest are fetched automatically by CMake (`FetchContent`); no
manual installation is required.

## Build, test, run

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
./build/debug/app/stockfih      # launch the GUI
```

Use the `release` preset for an optimized build.

## Testing

The engine is developed test-first with [GoogleTest](https://github.com/google/googletest).
Run the suite with CTest:

```sh
ctest --preset debug
```

Coverage includes board/FEN round-trips, per-piece move generation, the special
rules, check/checkmate/stalemate detection, search and evaluation, and a
[perft](https://www.chessprogramming.org/Perft_Results) suite that validates the
move generator against known node counts (starting position, Kiwipete, and the
en-passant and promotion reference positions). CI runs the build and tests on
Linux, Windows, and macOS for every pull request.

## Features

- Headless engine: board representation, FEN parsing/serialization, full legal
  move generation (including castling, en passant, and promotion), and
  check/checkmate/stalemate detection.
- Bot: minimax search with alpha-beta pruning and a positional evaluation
  (material, center control, mobility).
- GUI: a raylib chessboard with pieces and mouse interaction — select a piece to
  see its legal moves and play a full game against the bot.
