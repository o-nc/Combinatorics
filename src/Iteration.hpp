
#ifndef COMBINATORICS_ITERATION_HPP
#define COMBINATORICS_ITERATION_HPP

#include <atomic>
#include <generator>
#include <latch>
#include <numeric>
#include <optional>
#include <vector>

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


    // todo: atomic generators seem to be completely broken (atomic operations are not atomic at all). how to create a concurrent coroutine from scratch?
    /*
    std::generator<std::size_t> a_iterate(std::latch& all_done, std::size_t const up_to) {
        std::atomic<std::size_t> i{0};
        co_yield 0;
        while (true) {
            auto const current_index = i.fetch_add(1, std::memory_order_seq_cst);
            if (current_index >= up_to)
                break;
            co_yield current_index;
        }
        all_done.arrive_and_wait();
    }
    */

    // multithreaded functions to iterate over indices of integer
    struct iterate_atomic {
        std::atomic<std::size_t> i{0};
        // have argument in constructor ?
        std::optional<std::size_t> operator()(std::size_t const reel_size) {
            auto const current_index = i.fetch_add(1, std::memory_order_seq_cst);
            return current_index >= reel_size ? std::nullopt : std::optional<std::size_t>{current_index};
        }
        std::optional<std::vector<std::size_t>> operator()(std::vector<std::size_t> const& reel_sizes) {
            auto const c_sizes = cumulative_sizes(reel_sizes);
            auto index = operator()(c_sizes.front());
            while (index.has_value()) {
                return index_to_indices(c_sizes, index.value());
                index = operator()(c_sizes.front());
            }
            return std::nullopt;
        }
    };


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

    std::size_t spin(std::size_t const up_to); // random iteration
    // spin (random uniform over total_weight, then starting from 0, iterate until result <= cumulative_weight


    // permutations
    // combinations
}

#endif
