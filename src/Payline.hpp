
#ifndef COMBINATORICS_PAYLINE_HPP
#define COMBINATORICS_PAYLINE_HPP

#include "Display.hpp"

namespace combis {

    class Payline {
    	std::vector<position> const positions;
    public:
        Payline(std::vector<position> positions) : positions(std::move(positions)) {}

        out payout(Display const& d, std::vector<std::size_t> const& reels_stops = {}) const {
            auto const& s = d[positions[0], reels_stops];
            if (s.scattered)
                return 0;
            std::size_t count = 1;
            for (auto i = 1uz, size = positions.size(); i < size and (s.binary & d[positions[i], reels_stops].wild) != 0; ++i)
                ++count;
            // if we play the game or do MC, we want to get the outs as is
            // but if we iterate, we want to multiply the outs by the weight of all reel current stops that are part of the payline
            return s.outs[count];
        }
    };
    using Paylines = std::vector<Payline>;
}

#endif
