
#ifndef COMBINATORICS_ITERATION_HPP
#define COMBINATORICS_ITERATION_HPP

#include <atomic>
#include <generator>
#include <latch>
#include <numeric>
#include <optional>
#include <vector>

namespace combis {

    std::vector<std::size_t> cumulative_sizes(std::vector<std::size_t> reel_sizes /* take and modify a copy */);
    std::vector<std::size_t> index_to_indices(std::vector<std::size_t> const& c_sizes, std::size_t index);


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
    std::generator<std::size_t> iterate(std::size_t const reel_size);
    // iteration over more than one dimension
    std::generator<std::vector<std::size_t>> iterate(std::vector<std::size_t> const& reel_sizes);

    std::size_t spin(std::size_t const up_to); // random iteration
    // spin (random uniform over total_weight, then starting from 0, iterate until result <= cumulative_weight


    // permutations
    // combinations
}

#endif
