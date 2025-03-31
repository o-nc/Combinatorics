
#ifndef COMBINATORICS_REEL_HPP
#define COMBINATORICS_REEL_HPP

#include <numeric>
#include <vector>
#include "Symbol.hpp"

namespace combis {

    using position = std::size_t;
    using weight = std::size_t;
    struct Stop {
        Symbol const& s;
        weight const w;
        explicit Stop(Symbol const& s, weight const w = 1) : s(s), w(w) {}
        explicit Stop(std::size_t const s, weight const w = 1) : s(Symbol::get(s)), w(w) {}
        bool operator==(Stop const& other) const { return s == other.s and w == other.w; }
    };
    using Stops = std::vector<Stop>;
    inline Stops createStops(std::vector<std::size_t> const& ids, std::vector<weight> const& weights ={}) {
        // precondition: either weights is empty, in which case all weights are set to 1, or has the same size as ids
        Stops stops;
        stops.reserve(ids.size());
        for (std::size_t i = 0; i < ids.size(); ++i) {
            stops.emplace_back(ids[i], weights.empty() ? 1 : weights[i]);
        }
        return stops;
    }

    class Reel {
        Stops const stops;
    public:
        weight const total_weight;
        explicit Reel(Stops const& stops) : stops(stops), total_weight(std::accumulate(stops.cbegin(), stops.cend(), 0, [](weight const& acc, Stop const& s) { return acc + s.w; })) {}
        Stop const& operator[](std::size_t const offset) const { return stops[(offset) % stops.size()]; }
        std::size_t size() const { return stops.size(); }
    };
    using Reels = std::vector<Reel>;

}

#endif
