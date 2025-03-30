
#include <gtest/gtest.h>
#include "Reel.hpp"

using namespace combis;

TEST(ReelTest, CreationAndAccess) {
    Reel const reel(std::move(createStops( {0, 1, 2, 1, 2, 3, 4, 0, 4})));
    EXPECT_EQ(reel.total_weight, 9);
    EXPECT_EQ(reel[0], Stop(0));
    EXPECT_EQ(reel[5], Stop(3));
    EXPECT_EQ(reel[8], Stop(4));
    EXPECT_EQ(reel[9], Stop(0)); // modulo the size of the reel
}