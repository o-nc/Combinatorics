
#ifndef COMBINATORICS_DISPLAY_HPP
#define COMBINATORICS_DISPLAY_HPP

#include <bit>
#include <numeric>

#include "Reel.hpp"

namespace combis {

    class Display {
	public:
        using Height = std::size_t;
        using Heights = std::vector<Height>;
      	using Scattered = std::size_t;
    private:
        Reels& reels; // not const to allow the reels to change their current stop position
		Heights const heights; // vertical dimension on the number of symbols display vertically for each reel. could be a random variable, e.g. megaways

        // each position of the display should have a unique identifier (zero-indexed). e.g. position 4 on a 3 high 5 wide could be the mid symbol on the second reel.
        // i.e. positions [0, height_0-1] for the first reel, [height_0, height_0+height_1-1] for reel 2, [sum_{i=0}^{n-1} height_i, sum_{i=0}^n height_i - 1] for reel indexed n (zero-indexed)

    	std::pair<std::size_t, std::size_t> reel_index_offset(position const& p) const {
    		position current_p = 0;
            std::size_t reel_index = 0;
            while (p >= current_p + heights[reel_index])
            	current_p += heights[reel_index++];
            return std::make_pair(reel_index, p - current_p);
      	}

    public:
    	Scattered const scattered; // binary field of all the symbol indices that have scattered payout
        position const display_size; // the number of positions on the display
      	Display(Reels& reels, Heights const& heights, Scattered const scattered)
            : reels(reels), heights(heights), scattered(scattered), display_size(std::accumulate(heights.cbegin(), heights.cend(), 1uz)) {}

        std::pair<Symbol const&, weight const&> operator[](position const& p, std::vector<std::size_t> const& reels_stops = {}) const {
        	auto const [reel_index, display_offset] = reel_index_offset(p);
        	// keep the current position of all reels at 0, but change their position with an argument.
        	// make the reels' accesses thread-safe by getting rid of the shared data.
        	std::size_t const  reel_offset = reels_stops.empty() ? 0uz : reels_stops[reel_index];
        	// as the stop chosen is at offset 0, we must use the weight at offset 0, not the weight at the offset of the symbol
            return {reels[reel_index][reel_offset + display_offset].s, reels[reel_index][reel_offset].w};
        }
    };

}

#endif
