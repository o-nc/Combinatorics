
#ifndef COMBINATORICS_REEL_HPP
#define COMBINATORICS_REEL_HPP

#include <numeric>
#include <vector>
#include "Symbol.hpp"

namespace combis {

    class Reel {
    public:
        using Weight = std::size_t;
        struct Stop {
            Symbol const s;
            Weight const w;
            Stop(Symbol const s, Weight const w) : s(s), w(w) {}
            explicit Stop(std::size_t const s) : s(Symbol(s)), w(1) {}
            explicit Stop(Symbol const s) : s(s), w(1) {}
            bool operator==(Stop const& other) const { return s == other.s and w == other.w; }
        };
        using Stops = std::vector<Stop>;
    private:
        Stops const stops;
    public:
        Weight const total_weight;
        Reel(std::initializer_list<std::size_t> const& symbols) : stops(symbols.begin(), symbols.end()), total_weight(symbols.size()) {}
        explicit Reel(Stops const& stops) : stops(stops), total_weight(std::accumulate(stops.cbegin(), stops.cend(), 0, [](Weight const& acc, Stop const& s) { return acc + s.w; })) {}
        Stop const& operator[](std::size_t const index) const { return stops[index % stops.size()]; }
    };

}

#endif
