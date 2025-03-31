
#include <gtest/gtest.h>
#include "Display.hpp"
#include "TestingObjects.hpp"

using namespace combis;

TEST(DisplayTest, CreationAndAccess) {
    EXPECT_EQ(display_base[0].first, s7);
    EXPECT_EQ(display_base[1].first, s8);
    EXPECT_EQ(display_base[2].first, s4);
    EXPECT_EQ(display_base[3].first, s10);
}

TEST(DisplayTest, AccessWithReelsStops) {
    std::vector<std::size_t> reels_stops = {4,0,0,0,0};
    auto const& stest = display_base[0, reels_stops].first;
    EXPECT_EQ(stest, s8);
    EXPECT_EQ((display_base[1, reels_stops].first), s4);
    reels_stops[1] = 3;
    EXPECT_EQ((display_base[3, reels_stops].first), s1);
}