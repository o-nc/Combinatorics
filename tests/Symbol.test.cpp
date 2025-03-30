
#include <gtest/gtest.h>
#include "Symbol.hpp"

using namespace combis;

TEST(SymbolTest, BinaryRepresentation) {
    auto const& s0 = Symbol::get(0);
    auto const& s1 = Symbol::get(1);
    auto const& s2 = Symbol::get(2);
    auto const& s3 = Symbol::get(3);
    auto const& s4 = Symbol::get(4);
    EXPECT_EQ(s4.binary, 16);
    auto const& s5 = Symbol::get(5, {0, 0, 0, 2, 10, 100}, {0, 1, 6});
    EXPECT_EQ(s5.id, 5);
    EXPECT_EQ(s5.binary, 32);
    EXPECT_EQ(s5.wild, 99);
    EXPECT_EQ(s5.outs[5], 100);
}
