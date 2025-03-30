
#include <gtest/gtest.h>
#include "Display.hpp"

using namespace combis;

TEST(DisplayTest, CreationAndAccess) {

    auto const& s0 = Symbol::get(0); // unused symbol
    auto const& s1 = Symbol::get(1, {0, 0, 0, 5, 10, 50});
    auto const& s2 = Symbol::get(2, {0, 0, 0, 5, 10, 50});
    auto const& s3 = Symbol::get(3, {0, 0, 0, 5, 10, 50});
    auto const& s4 = Symbol::get(4, {0, 0, 0, 5, 10, 50});
    auto const& s5 = Symbol::get(5, {0, 0, 0, 5, 20, 75});
    auto const& s6 = Symbol::get(6, {0, 0, 0, 5, 25, 100});
    auto const& s7 = Symbol::get(7, {0, 0, 0, 5, 25, 100});
    auto const& s8 = Symbol::get(8, {0, 0, 0, 5, 30, 100});
    auto const& s9 = Symbol::get(9, {0, 0, 2, 10, 50, 150});
    auto const& s10 = Symbol::get(10, {}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    auto const& s11 = Symbol::get(11);
    auto const& s12 = Symbol::get(12, {0, 0, 0, 2, 15, 100}); // this should be time the total bet, all the other are time the bet per line

    std::vector<std::size_t> ids {7,8,4,1,8,4,5,1,4,8,1,7,6,4,9,7,5,12,2,3,8,4,2,8,1,7,2,8,9,2,7,6,2,1,4,7,2,1,3,6,2,11,11,4,9,7,5,1,7,8,2,4,9,1,8,2,1,7,12,11,11,11,4,1};
    Reel reel1(std::move(createStops(std::move(ids), {})));
    EXPECT_EQ(reel1.total_weight, 64);
    ids = {10,3,2,1,6,4,5,6,3,5,8,12,10,1,9,3,6,5,8,6,10,2,5,9,1,5,10,12,3,2,7,4,3,6,5,9,3,10,5,6,10,3,6,5,11,11,11,8,5,7,6,11,11,3,9,8,5,3,10,9,3,6,8,3,9,8};
    Reel reel2(std::move(createStops(std::move(ids), {})));
    Reels reels {std::move(reel1), std::move(reel2)};
    Display::Heights const heights(2, 3); // two reels, each of height three
    std::size_t const scattered = (1 << 11) | (1 << 12);
    Display display(reels, heights, scattered);

    EXPECT_EQ(display[0].first, s7);
    EXPECT_EQ(display[1].first, s8);
    EXPECT_EQ(display[2].first, s4);
    EXPECT_EQ(display[3].first, s10);

    reels[0].current = 4;
    EXPECT_EQ(display[0].first, s8);
    EXPECT_EQ(display[1].first, s4);
    EXPECT_EQ(display[2].first, s5);
}
