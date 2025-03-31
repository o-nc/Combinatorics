
#include <gtest/gtest.h>
#include "Payline.hpp"

#include "TestingObjects.hpp"

using namespace combis;

TEST(PaylineTest, PaylineCredit) {
    display_base.set_position(2, 3);
    display_base.set_position(3, 11);
    display_base.set_position(4, 8);
    Payline payline({0,3,7,9,14}); // top row for reels 1, 2, 4, mid row for reel 3, bottom row for reel 5
    EXPECT_EQ(payline.payout(display_base), 100);
    display_base.set_position(4, 0);
    EXPECT_EQ(payline.payout(display_base), 25);
    display_base.set_position(3, 0);
    EXPECT_EQ(payline.payout(display_base), 5);
    display_base.set_position(2, 1);
    EXPECT_EQ(payline.payout(display_base), 0);
    display_base.set_position(2, 0);
    EXPECT_EQ(payline.payout(display_base), 5); // hit a wild
}
