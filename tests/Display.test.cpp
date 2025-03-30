
#include <gtest/gtest.h>
#include "Display.hpp"
#include "TestingObjects.hpp"

using namespace combis;

TEST(DisplayTest, CreationAndAccess) {
    EXPECT_EQ(display_base[0].first, s7);
    EXPECT_EQ(display_base[1].first, s8);
    EXPECT_EQ(display_base[2].first, s4);
    EXPECT_EQ(display_base[3].first, s10);

    display_base.set_position(0, 4);
    EXPECT_EQ(display_base[0].first, s8);
    EXPECT_EQ(display_base[1].first, s4);
    EXPECT_EQ(display_base[2].first, s5);
    display_base.set_position(0, 0);
}
