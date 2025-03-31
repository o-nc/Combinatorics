
#include <gtest/gtest.h>
#include "Payline.hpp"

#include "TestingObjects.hpp"

using namespace combis;

TEST(PaylineTest, PaylineCredit) {
    std::vector<std::size_t> reels_stops = {0,0,3,11,8};
    //display_base.set_position(2, 3);
    //display_base.set_position(3, 11);
    //display_base.set_position(4, 8);
    Payline payline({0,3,7,9,14}); // top row for reels 1, 2, 4, mid row for reel 3, bottom row for reel 5
    EXPECT_EQ(payline.payout(display_base, reels_stops), 100);
    //display_base.set_position(4, 0);
    reels_stops[4] = 0;
    EXPECT_EQ(payline.payout(display_base, reels_stops), 25);
    reels_stops[3] = 0;
    //display_base.set_position(3, 0);
    EXPECT_EQ(payline.payout(display_base, reels_stops), 5);
    reels_stops[2] = 1;
    //display_base.set_position(2, 1);
    EXPECT_EQ(payline.payout(display_base, reels_stops), 0);
    reels_stops[2] = 0;
    //display_base.set_position(2, 0);
    EXPECT_EQ(payline.payout(display_base, reels_stops), 5); // hit a wild
}
