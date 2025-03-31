
#include "Iteration.hpp"


namespace combis {

    std::vector<std::size_t> cumulative_sizes(std::vector<std::size_t> reel_sizes /* take and modify a copy */) {
        std::inclusive_scan(reel_sizes.rbegin(), reel_sizes.rend(), reel_sizes.rbegin(), std::multiplies<std::size_t>());
        return reel_sizes;
    }

    std::vector<std::size_t> index_to_indices(std::vector<std::size_t> const& c_sizes, std::size_t index) {
        auto const num_reels = c_sizes.size();
        std::vector<std::size_t> indices(num_reels, 0);
        for (auto i = 0; i + 1 < num_reels; ++i) {
            indices[i] = index / c_sizes[i + 1];
            index -= indices[i] * c_sizes[i + 1];
        }
        indices.back() = index;
        return indices;
    }

    // functions for a single thread
    std::generator<std::size_t> iterate(std::size_t const reel_size) {
    	// from zero to the last, non-concurrent safe
    	for (auto i = 0uz; i < reel_size; ++i)
        	co_yield i;
    }
    // iteration over more than one dimension
    std::generator<std::vector<std::size_t>> iterate(std::vector<std::size_t> const& reel_sizes) {
        auto const c_sizes = cumulative_sizes(reel_sizes);
        for (auto index : iterate(c_sizes.front()))
            co_yield index_to_indices(c_sizes, index);
    }

}

