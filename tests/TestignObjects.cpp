
#include "TestingObjects.hpp"

using namespace combis;

auto const& s0 = Symbol::get(0); // unused
Symbol const& s1 = Symbol::get(1, {0, 0, 0, 5, 10, 50});
Symbol const& s2 = Symbol::get(2, {0, 0, 0, 5, 10, 50});
Symbol const& s3 = Symbol::get(3, {0, 0, 0, 5, 10, 50});
Symbol const& s4 = Symbol::get(4, {0, 0, 0, 5, 10, 50});
Symbol const& s5 = Symbol::get(5, {0, 0, 0, 5, 20, 75});
Symbol const& s6 = Symbol::get(6, {0, 0, 0, 5, 25, 100});
Symbol const& s7 = Symbol::get(7, {0, 0, 0, 5, 25, 100});
Symbol const& s8 = Symbol::get(8, {0, 0, 0, 5, 30, 100});
Symbol const& s9 = Symbol::get(9, {0, 0, 2, 10, 50, 150});
Symbol const& s10 = Symbol::get(10, {}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
Symbol const& s11 = Symbol::get(11);
Symbol const& s12 = Symbol::get(12, {0, 0, 0, 2, 15, 100}); // this should be time the total bet, all the other are time the bet per line

std::vector<std::size_t> ids1 {7,8,4,1,8,4,5,1,4,8,1,7,6,4,9,7,5,12,2,3,8,4,2,8,1,7,2,8,9,2,7,6,2,1,4,7,2,1,3,6,2,11,11,4,9,7,5,1,7,8,2,4,9,1,8,2,1,7,12,11,11,11,4,1};
Reel reel1(std::move(createStops(std::move(ids1), {})));
std::vector<std::size_t> ids2 = {10,3,2,1,6,4,5,6,3,5,8,12,10,1,9,3,6,5,8,6,10,2,5,9,1,5,10,12,3,2,7,4,3,6,5,9,3,10,5,6,10,3,6,5,11,11,11,8,5,7,6,11,11,3,9,8,5,3,10,9,3,6,8,3,9,8};
Reel reel2(std::move(createStops(std::move(ids2), {})));
std::vector<std::size_t> ids3 = {4,10,3,9,7,8,3,1,7,3,5,9,8,10,5,8,4,10,5,7,2,11,11,8,10,5,9,6,10,11,11,11,7,10,12,3,8,10,3,6,9,5,6,1,12,5,6,3,10,6,7,3,5,6,3,2,6,3,4,5,3,4,6,5};
Reel reel3(std::move(createStops(std::move(ids3), {})));
std::vector<std::size_t> ids4 = {5,2,8,3,4,6,10,11,11,3,1,7,2,6,7,1,4,2,10,6,2,5,6,4,3,8,11,11,11,6,8,7,1,4,6,1,5,7,10,9,4,10,7,9,2,3,6,4,3,7,4,3,12,1,8,10,6,12,3,9,6,5,4,10};
Reel reel4(std::move(createStops(std::move(ids4), {})));
std::vector<std::size_t> ids5 = {2,5,3,8,2,1,4,2,3,6,7,12,11,11,11,3,5,4,10,1,4,6,3,7,6,1,8,7,3,5,7,2,5,12,4,9,7,6,9,7,12,2,4,3,9,5,4,8,9,4,5,8,10,5,2,10,6,5,8,1,2,6,8,7,4,1,2,7,5,2,6,9,3,1,2,4,5,1,10,3,9,5,11,11,6};
Reel reel5(std::move(createStops(std::move(ids5), {})));
std::vector<std::size_t> ids6 = {6,7,8,9,7,8,6,9,8,7,9,11,11,11,7,8,9,7,12,8,7,6,9,8,11,11,7,8,12,7,8};
Reel reel6(std::move(createStops(std::move(ids6), {})));
std::vector<std::size_t> ids7 = {9,10,6,8,7,6,10,12,7,6,10,8,6,9,10,6,9,10,12,11,11,11,9,6,8,9,11,11,6,10,8,6,9,10,6};
Reel reel7(std::move(createStops(std::move(ids7), {})));
std::vector<std::size_t> ids8 = {6,9,7,6,8,7,10,6,12,8,10,6,11,11,9,8,10,7,6,10,8,6,10,12,8,9,6,7,11,11,11,10,6,7,10};
Reel reel8(std::move(createStops(std::move(ids8), {})));
std::vector<std::size_t> ids9 = {11,11,11,10,7,6,8,9,10,6,9,10,7,6,12,7,9,6,7,8,6,9,8,6,7,10,6,12,9,6,11,11,7,6};
Reel reel9(std::move(createStops(std::move(ids9), {})));
std::vector<std::size_t> ids10 = {12,9,10,7,6,11,11,9,10,12,7,9,6,8,7,11,11,11,6,8,7,6,10,7,9,10,7,8,9,12,8,7,6,8,7,10,6,8,9,10};
Reel reel10(std::move(createStops(std::move(ids10), {})));

Reels reels_1_5 {std::move(reel1), std::move(reel2), std::move(reel3), std::move(reel4), std::move(reel5)};
Reels reels_6_10 {std::move(reel6), std::move(reel7), std::move(reel8), std::move(reel9), std::move(reel10)};
Display::Heights const heights(5, 3); // five reels, each of height three
std::size_t const scattered = (1 << 11) | (1 << 12);

Display display_base(reels_1_5, heights, scattered);
Display display_free(reels_6_10, heights, scattered);
