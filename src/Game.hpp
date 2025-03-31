
#ifndef COMBINATORICS_GAME_HPP
#define COMBINATORICS_GAME_HPP

#include <generator>
#include <latch>
#include <thread>

#include "Iteration.hpp"
#include <LockfreeCounter.hpp>
#include "Payline.hpp"

namespace combis {

	template <std::size_t num_reels>
    class Game {
        Paylines const& paylines;
        Display& display;
        std::size_t const bet_size;

        out scattered_payout(std::vector<std::size_t> const& reels_stops, std::size_t const symbol_id) const {
      		std::size_t count = 0uz;
      		for (position p = 1uz; p < display.display_size; ++p) {
		        if (auto const& s = display[p, reels_stops]; s.id != symbol_id)
      				continue;
      			++count;
      		}
      		return Symbol::get(symbol_id).outs[count] * bet_size; // time the total bet, not time the bet per payline
      	}

        std::generator<std::size_t> get_scattered() const {
			auto s = display.scattered;
            while (s != 0) {
				auto const power = std::bit_floor(s);
            	s ^= power;
                auto const id = std::countr_zero(power);
                co_yield id;
            }
        }

    	out payout(std::vector<std::size_t> const& reels_stops = {}) const {
        	out result = 0uz;
        	for (auto const& payline : paylines)
        		result += payline.payout(display, reels_stops);

        	// Bonus/Free games and scattered symbols are not fully implemented.
        	//for (auto const symbol_id : get_scattered())
        	//	result += scattered_payout(reels_stops, symbol_id); // should also trigger free/bonus games
        	return result;
        }

    	void multithread_iteration(Counter& c, iterate_atomic& ia, std::latch& done, std::vector<std::size_t> const& reel_sizes) const {
        	std::vector<std::size_t> const first_two = {reel_sizes.cbegin(), std::next(reel_sizes.cbegin(), 2)};
        	Counter::unordered_map thread_local_counter;
        	auto indices = ia(first_two); // has 5 dimensions
        	while (indices.has_value()) {
        		singlethread_iteration(thread_local_counter, reel_sizes, indices.value());
        		indices = ia(first_two);
        	}
        	for (auto const [key, value] : thread_local_counter)
        		c.increment(key, value);
        	done.count_down();
        }

		static std::generator<std::vector<std::size_t> const&> remaining_reels_stops(std::vector<std::size_t>const& reel_sizes,  std::vector<std::size_t>& reels_stops) {
        	// not pretty. something better should be made.
        	if constexpr (num_reels < 3)
        		co_return;
			for (auto const reel3_stop : iterate(reel_sizes[2])) {
				reels_stops[2] = reel3_stop;
				if constexpr (num_reels == 3)
					co_yield reels_stops;
				else {
					for (auto const reel4_stop : iterate(reel_sizes[3])) {
						reels_stops[3] = reel4_stop;
						if constexpr (num_reels == 4)
							co_yield reels_stops;
						else {
							for (auto const reel5_stop : iterate(reel_sizes[4])) {
								reels_stops[4] = reel5_stop;
								co_yield reels_stops;
							}
						}
					}
				}
			}
		}

    	void singlethread_iteration(Counter::unordered_map& c, std::vector<std::size_t> const& reel_sizes, std::vector<std::size_t>& reels_stops) const {
        	reels_stops.resize(reel_sizes.size());
    		for (auto const& stops: remaining_reels_stops(reel_sizes, reels_stops)) {
    			auto const outs = payout(stops);
    			auto const weight = display.get_weight(stops);
    			c[outs] += weight;
    		}
        }

    public:
    	Game(Paylines const& paylines, Display& display, std::size_t const bet_size) : paylines(paylines), display(display), bet_size(bet_size) {}

    	void iterate_all(Counter& c) {
    		iterate_atomic ia;
    		std::vector<std::size_t> const& reel_sizes = display.get_reels_sizes();
    		auto const num_threads = std::thread::hardware_concurrency();
    		std::latch done(num_threads);
    		for (std::vector<std::jthread> threads(num_threads); auto& thread : threads)
    			thread = std::jthread(&Game::multithread_iteration, this, std::ref(c), std::ref(ia), std::ref(done), std::cref(reel_sizes));
    		done.wait();
    	}

    };

}

#endif
