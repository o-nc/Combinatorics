
#include <gtest/gtest.h>
#include "Reel.hpp"

using namespace combis;

TEST(ReelTest, CreationAndAccess) {
    Reel const reel {0, 1, 2, 1, 2, 3, 4, 0, 4};
    EXPECT_EQ(reel.total_weight, 9);
    EXPECT_EQ(reel[0], Reel::Stop(0));
    EXPECT_EQ(reel[5], Reel::Stop(3));
}