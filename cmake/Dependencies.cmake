# Third-party dependencies, fetched at configure time so no system install is
# required. Each dependency is guarded by the option that needs it.

include(FetchContent)

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
