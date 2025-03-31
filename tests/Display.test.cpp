
#include <gtest/gtest.h>
#include "Display.hpp"
#include "TestingObjects.hpp"

using namespace combis;

TEST(DisplayTest, CreationAndAccess) {
    EXPECT_EQ(display_base[0], s7);
    EXPECT_EQ(display_base[1], s8);
    EXPECT_EQ(display_base[2], s4);
    EXPECT_EQ(display_base[3], s10);
}

TEST(DisplayTest, AccessWithReelsStops) {
    std::vector<std::size_t> reels_stops = {4,0,0,0,0};
    EXPECT_EQ((display_base[0, reels_stops]), s8);
    EXPECT_EQ((display_base[1, reels_stops]), s4);
    reels_stops[1] = 3;
    EXPECT_EQ((display_base[3, reels_stops]), s1);
}