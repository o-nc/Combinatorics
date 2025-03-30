
#include <gtest/gtest.h>
#include "Symbol.hpp"
#include "TestingObjects.hpp"

using namespace combis;

TEST(SymbolTest, BinaryRepresentation) {
    EXPECT_EQ(s4.id, 4);
    EXPECT_EQ(s4.binary, 16);
    EXPECT_EQ(s9.outs[5], 150);
    EXPECT_EQ(s10.id, 10);
    EXPECT_EQ(s10.binary, 1024);
    EXPECT_EQ(s10.wild, 2046); // symbol 0 is unused
}
