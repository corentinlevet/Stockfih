# Third-party dependencies, fetched at configure time so no system install is
# required. Each dependency is guarded by the option that needs it.

include(FetchContent)

# raylib -- graphics/windowing library used by the GUI application only.
if(STOCKFIH_BUILD_APP)
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
  )
  # We only need the library, not raylib's bundled examples or games.
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(raylib)
endif()

# GoogleTest -- unit-test framework used by the headless engine test runner.
if(STOCKFIH_BUILD_TESTS)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
    GIT_SHALLOW TRUE
  )
  # Keep the MSVC runtime consistent between gtest and our targets.
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()
