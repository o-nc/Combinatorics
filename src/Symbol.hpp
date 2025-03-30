
#ifndef COMBINATORICS_SYMBOL_HPP
#define COMBINATORICS_SYMBOL_HPP

#include <numeric>

namespace combis {

    struct Symbol {
        std::size_t const symbol;
        std::size_t const binary;
        std::size_t const wild;
        explicit Symbol(std::size_t const s) : symbol(s), binary(1 << s), wild(binary) {}
        Symbol(std::size_t const s, std::vector<std::size_t> const& wild_for) : symbol(s), binary(1 << s), wild(to_wild(s, wild_for)) {}
        bool operator==(Symbol const& other) const { return symbol == other.symbol and wild == other.wild; }
    private:
        static std::size_t to_wild(std::size_t const s, std::vector<std::size_t> const& wild_for) {
            std::size_t const wild = 1 << s;
            return std::accumulate(wild_for.begin(), wild_for.end(), wild,
                [](std::size_t const& b, std::size_t const& w) { return b | (1 << w); });
        }
    };

}

#endif
