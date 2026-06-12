#include <gtest/gtest.h>

#include "stockfih/version.hpp"

// Smoke test that proves the engine library links and the test harness runs.
TEST(Version, IsNotEmpty) {
  EXPECT_FALSE(stockfih::version().empty());
}
