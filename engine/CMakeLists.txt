add_library(stockfih_engine STATIC
  src/version.cpp
  src/board.cpp
  src/fen.cpp
  src/movegen.cpp
  src/gamestate.cpp
  src/rules.cpp
)

target_include_directories(stockfih_engine
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(stockfih_engine PUBLIC cxx_std_20)

# The engine is intentionally free of any graphics dependency so it can be
# unit-tested headlessly.
