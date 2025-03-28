
#include <latch>
#include <thread>
#include <gtest/gtest.h>
#include "LockfreeCounter.hpp"

using namespace combis;

class LockfreeCounterTestSuite : public testing::TestWithParam<std::array<uint64_t, 4>> {};

TEST_P(LockfreeCounterTestSuite, ConcurrencyTest)
{
    using Counter = LockfreeCounter<uint64_t, uint64_t>;
    Counter counter {};
    auto const [key, value, repeat, num_keys] = GetParam();
    auto const num_threads = std::thread::hardware_concurrency();
    std::latch start_latch(num_threads), end_latch(num_threads);
    auto incrementer = [&](uint64_t local_repeat) {
        start_latch.arrive_and_wait(); // don't want some thread to start running before all the threads are ready
        while (local_repeat--)
            for (uint64_t key_num = key; key_num < key+num_keys; ++key_num)
                counter.increment(key_num, value);
        end_latch.count_down();
    };

    std::vector<std::jthread> threads(num_threads);
    for (auto& thread : threads)
        thread = std::jthread(incrementer, repeat);
    end_latch.wait(); // enforces a precondition: no thread is modifying counter while converting to std::unordered_map
    auto const dict = static_cast<Counter::unordered_map>(counter);

    EXPECT_EQ(dict.size(), num_keys);
    auto const total_value = value * repeat * num_threads;
    for (uint64_t key_num = key; key_num < key+num_keys; ++key_num)
        EXPECT_EQ(dict.at(key_num), total_value);
}

LockfreeCounterTestSuite::ParamType OneKeyAndHighRepeat({0, 1, 10'000, 1});
LockfreeCounterTestSuite::ParamType HighBucketContentionAndLowRepeat({0, 1, 10, 10'000});
INSTANTIATE_TEST_SUITE_P(Moments, LockfreeCounterTestSuite, testing::Values(OneKeyAndHighRepeat, HighBucketContentionAndLowRepeat));