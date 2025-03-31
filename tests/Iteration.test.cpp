
#include <algorithm>
#include <format>
#include <latch>
#include <optional>
#include <thread>
#include <unordered_set>
#include <vector>
#include <gtest/gtest.h>
#include "Iteration.hpp"

using namespace combis;

TEST(IterationTest, IterateAtomic) {
    constexpr std::size_t up_to= 10'000uz;
    auto const num_threads = std::thread::hardware_concurrency();
    iterate_atomic ia;

    std::latch start_latch(num_threads), end_latch(num_threads);
    std::vector<std::unordered_set<std::size_t>> iterations(num_threads);
    //auto gen = a_iterate(end_latch, up_to);
    //gen.begin(); // get ready
    auto thread_local_iterator = [&](std::size_t const i) {
        auto& set = iterations[i];
        start_latch.arrive_and_wait();
        auto index = ia(up_to);
        while (index.has_value()) {
            set.insert(index.value());
            index = ia(up_to);
        }
        end_latch.count_down();
        //for (auto const index: gen)
        //    set.insert(index);
    };

    std::vector<std::jthread> threads(num_threads);
    for (auto i = 0uz; i < num_threads; ++i)
        threads[i] = std::jthread(thread_local_iterator, i);
    end_latch.wait();
    std::unordered_set<std::size_t> all_iterations;
    for (auto& iteration : iterations)
        all_iterations.merge(iteration);
    EXPECT_EQ(all_iterations.size(), up_to) << std::format("size()={}", all_iterations.size());
    for (auto i = 0uz; i < up_to; ++i)
        EXPECT_TRUE(all_iterations.contains(i)) << std::format("i={}", i);
}

TEST(IterationTest, IndexToIndices) {
    std::vector<std::size_t> const reel_sizes = {100, 97, 91, 79}, expected_c_sizes = {69'733'300, 697'333, 7'189, 79};
    auto const c_sizes = cumulative_sizes(reel_sizes);
    EXPECT_EQ(c_sizes, expected_c_sizes);

    std::vector<std::size_t> expected_indices = {0, 0, 0, 0};
    EXPECT_EQ(expected_indices, index_to_indices(c_sizes, 0));
    expected_indices = {0, 0, 0, 78};
    EXPECT_EQ(expected_indices, index_to_indices(c_sizes, 78));
    expected_indices = {0, 0, 1, 0};
    EXPECT_EQ(expected_indices, index_to_indices(c_sizes, 79));
    expected_indices = {0, 96, 90, 78};
    EXPECT_EQ(expected_indices, index_to_indices(c_sizes, 697'332));
    expected_indices = {1, 0, 0, 0};
    EXPECT_EQ(expected_indices, index_to_indices(c_sizes, 697'333));
    expected_indices = {1, 0, 0, 1};
    EXPECT_EQ(expected_indices, index_to_indices(c_sizes, 697'334));
}

TEST(IterationTest, MultiReelsIteration) {
    std::vector<std::size_t> const reel_sizes = {2, 3, 4};
    std::vector<std::vector<std::size_t>> const expected_indices = {
        {0,0,0}, {0,0,1}, {0,0,2}, {0,0,3},
        {0,1,0}, {0,1,1}, {0,1,2}, {0,1,3},
        {0,2,0}, {0,2,1}, {0,2,2}, {0,2,3},
        {1,0,0}, {1,0,1}, {1,0,2}, {1,0,3},
        {1,1,0}, {1,1,1}, {1,1,2}, {1,1,3},
        {1,2,0}, {1,2,1}, {1,2,2}, {1,2,3},
    };
    auto i = 0uz;
    for (auto const& indices : iterate(reel_sizes)) {
        EXPECT_EQ(expected_indices[i], indices);
        ++i;
    }
}

TEST(IterationTest, MultiReelsAtomicIteration) {
    constexpr auto num_threads = 4;
    iterate_atomic ia;
    std::latch start_latch(num_threads), end_latch(num_threads);
    std::vector<std::vector<std::vector<std::size_t>>> iterations(num_threads);
    std::vector<std::size_t> const reel_sizes = {2, 3, 4};
    auto thread_local_iterator = [&](std::size_t const i) {
        auto& visited = iterations[i];
        start_latch.arrive_and_wait();
        auto indices = ia(reel_sizes);
        while (indices.has_value()) {
            visited.push_back(indices.value());
            indices = ia(reel_sizes);
        }
        end_latch.count_down();
    };

    std::vector<std::jthread> threads(num_threads);
    for (auto i = 0uz; i < num_threads; ++i)
        threads[i] = std::jthread(thread_local_iterator, i);
    end_latch.wait();
    std::vector<std::vector<std::size_t>> all_iterations;
    for (auto& iteration : iterations)
        for (auto& indices : iteration)
            all_iterations.push_back(std::move(indices));
    std::ranges::sort(all_iterations);
    std::vector<std::vector<std::size_t>> expected_indices = {
        {0,0,0}, {0,0,1}, {0,0,2}, {0,0,3},
        {0,1,0}, {0,1,1}, {0,1,2}, {0,1,3},
        {0,2,0}, {0,2,1}, {0,2,2}, {0,2,3},
        {1,0,0}, {1,0,1}, {1,0,2}, {1,0,3},
        {1,1,0}, {1,1,1}, {1,1,2}, {1,1,3},
        {1,2,0}, {1,2,1}, {1,2,2}, {1,2,3},
    };
    EXPECT_EQ(all_iterations.size(), expected_indices.size()) << std::format("size()={}", all_iterations.size());
    for (auto i = 0uz, size = expected_indices.size(); i < size; ++i)
        EXPECT_EQ(all_iterations[i], expected_indices[i]) << std::format("i={}", i);
}