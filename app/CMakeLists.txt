add_executable(stockfih
  src/main.cpp
  src/renderer.cpp
  src/input.cpp
)

target_link_libraries(stockfih PRIVATE stockfih_engine raylib)

# raylib provides the windowing/rendering; the engine stays graphics-free.
