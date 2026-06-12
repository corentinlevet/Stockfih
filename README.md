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

## Status

Under active development, tracked issue-by-issue. See the GitHub issues for the
current roadmap.
