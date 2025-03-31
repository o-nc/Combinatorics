
#ifndef COMBINATORICS_SYMBOL_HPP
#define COMBINATORICS_SYMBOL_HPP

#include <numeric>

namespace combis {

    using out = std::size_t;

    struct Symbol {
        std::size_t const id; // a unique identifier
        std::size_t const binary;
        std::size_t const wild; // binary or of the binary representation of each symbol that this can be a wild for
        bool const scattered; // does the symbol has a scattered payout?
        std::vector<out> const outs;

        // deleted default constructor, copy constructor and copy assignment to create singleton
        Symbol() = delete;
        Symbol(Symbol const&) = delete;
        // this class is not a true singleton, as we want one static symbol per id.
        // to store the symbols, need a way to place them in a vector... so here is a public move constructor...
        Symbol(Symbol&& s) noexcept : id(s.id), binary(s.binary), wild(s.wild), scattered(s.scattered), outs(s.outs) {}
        void operator=(Symbol const&) = delete;
        bool operator==(Symbol const& other) const { return id == other.id and wild == other.wild; }

        static Symbol const& get(std::size_t const id, std::vector<out> outs = {}, std::vector<std::size_t> wild_for = {}, bool const scattered = false) {
            // precondition, the symbols are created in increasing order of id, without skipping an id.
            static std::vector<Symbol> symbols;
            if (symbols.empty())
                symbols.reserve(100); // prevent reallocation when the vector grows, which would deallocate all the previous references
            static std::size_t size = 0;
            if (id < size)
                return symbols[id];
            Symbol s(id, std::move(outs), std::move(wild_for), scattered);
            symbols.push_back(std::move(s));
            ++size;
            return symbols.back();
        }
    private:
        Symbol(std::size_t const id, std::vector<out>&& outs, std::vector<std::size_t>&& wild_for, bool const scattered = false)
            : id(id), binary(1 << id), wild(to_wild(id, std::move(wild_for))), scattered(scattered), outs(std::move(outs)) {}

        static std::size_t to_wild(std::size_t const id, std::vector<std::size_t> wild_for) {
            std::size_t const wild = 1 << id;
            return std::accumulate(wild_for.begin(), wild_for.end(), wild,
                [](std::size_t const& b, std::size_t const& w) { return b | (1 << w); });
        }
    };

}

#endif
