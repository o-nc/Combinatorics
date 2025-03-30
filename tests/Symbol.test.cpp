
#include <gtest/gtest.h>
#include "Symbol.hpp"

using namespace combis;

TEST(SymbolTest, BinaryRepresentation) {
    Symbol s {4};
    EXPECT_EQ(s.binary, 16);
    Symbol s_wild {5, {0, 1, 6}};
    EXPECT_EQ(s_wild.symbol, 5);
    EXPECT_EQ(s_wild.binary, 32);
    EXPECT_EQ(s_wild.wild, 99);
}
