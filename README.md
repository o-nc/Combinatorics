## A small program to simulate slot machines

I spent a few hours over the last few days creating this multithreaded library to simulate and compute statistics for slot machines.  
This beta/demo version can do the following:
- Iterate, concurrently, over all permutations of up to five weighted reels.
- Create a display with variable height for each reel.
- The reels show symbols that can be wild, scattered or on a payline.
- For each wild symbol, one can choose which other symbols are matched by this wild.
- Create left-aligned or right-aligned paylines.
- Attach payouts tiers to any kind of symbols, e.g. payout of 5 credits if the first three symbols on a payline are a match for a given symbol.
- Compute the sum of all payouts over all paylines.

The library was built with modularity in mind, so that different games with different features could be easily supported.
Many unit tests are present in the tests/ directory.

Some features are not yet implemented, e.g. free games, bonus games, more statistics, etc. The library was built with them in mind however, and they could be added without problems.

### Example

See tests/TestingObjects.cpp and tests/Game.test.cpp to get an idea of how the library can be used. 
A unit test in Game.test.cpp replicates a game design for which statistics were computed in Excel. 
The iteration over all reels' stops yield the same number of outs and the same RTP for the payline of this base game.
This iteration (over a cycle of length 1470627840) currently takes around 25 seconds using 32 threads on as many physical cores.
The library has not been optimized or even profiled yet, however. So the 25 seconds currently needed are a bound on the time to iterate over a game of such size, not an estimate of the time needed for an optimized library.
