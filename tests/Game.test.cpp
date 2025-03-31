
#include <gtest/gtest.h>
#include "Game.hpp"
#include "TestingObjects.hpp"

using namespace combis;

TEST(GameTest, SmallGame) {
    Counter c;
    game_small.iterate_all(c);
    Counter::unordered_map um(c);
    auto const num_combos = std::accumulate(um.begin(), um.end(), 0uz, [](weight const& acc, auto const& kv) { return acc + kv.second; });
    EXPECT_EQ(num_combos, display_small.get_total_weight());
    auto const cycle_length = display_small.get_total_weight();
    EXPECT_EQ(cycle_length, 6480);
    auto const num_outs = std::accumulate(um.begin(), um.end(), 0uz, [](weight const& acc, auto const& kv) { return acc + kv.first * kv.second; });
    EXPECT_EQ(num_outs, 1217);
}

TEST(GameTest, ExcelGame) {
    auto const cycle_length = display_base.get_total_weight();
    EXPECT_EQ(cycle_length, 1470627840);

    Counter c;
    game_base.iterate_all(c);
    Counter::unordered_map um(c);

    auto const num_combos = std::accumulate(um.begin(), um.end(), 0uz, [](weight const& acc, auto const& kv) { return acc + kv.second; });
    EXPECT_EQ(num_combos, display_base.get_total_weight());
    EXPECT_EQ(cycle_length, num_combos);

    auto const num_outs = std::accumulate(um.begin(), um.end(), 0uz, [](weight const& acc, auto const& kv) { return acc + kv.first * kv.second; });
    EXPECT_EQ(num_outs, 452663900);
    EXPECT_NEAR(static_cast<double>(num_outs) / cycle_length, 0.30780316249147, 1e-8);
}