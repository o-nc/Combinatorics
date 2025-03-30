
#include <gtest/gtest.h>
#include "Reel.hpp"

using namespace combis;

TEST(ReelTest, CreationAndAccess) {
    auto const& s0 = Symbol::get(0);
    auto const& s1 = Symbol::get(1);
    auto const& s2 = Symbol::get(2);
    auto const& s3 = Symbol::get(3);
    auto const& s4 = Symbol::get(4);
    Reel const reel(std::move(createStops( {0, 1, 2, 1, 2, 3, 4, 0, 4}, {})));
    EXPECT_EQ(reel.total_weight, 9);
    EXPECT_EQ(reel[0], Stop(0));
    EXPECT_EQ(reel[5], Stop(3));
    EXPECT_EQ(reel[8], Stop(4));
    EXPECT_EQ(reel[9], Stop(0)); // modulo the size of the reel
}