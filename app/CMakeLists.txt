add_executable(stockfih
  src/main.cpp
)

target_link_libraries(stockfih PRIVATE stockfih_engine)

# raylib is linked here in issue #2; the engine stays graphics-free.
