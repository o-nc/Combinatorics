
#ifndef COMBINATORICS_PAYLINE_HPP
#define COMBINATORICS_PAYLINE_HPP

#include "Display.hpp"

namespace combis {

    class Payline {
    	std::vector<position> const positions;
    public:
        Payline(std::vector<position> positions) : positions(std::move(positions)) {} ;
        out payout(Display const& d, bool const iterate = false) const {
            auto const& [s, w]= d[positions[0]];
            std::size_t count = 1, weight = w;
            for (auto i = 1uz, size = positions.size(); i < size and (s.binary & d[positions[i]].first.wild) != 0; ++i) {
                weight *=  d[positions[i]].second;
                ++count;
            }
            // if we play the game or do MC, we want to get the outs as is
            // but if we iterate, we want to multiply the outs by the weight of all reel current stops that are part of the payline
            return iterate ? s.outs[count] * weight : s.outs[count];
        }
    };
}

#endif
