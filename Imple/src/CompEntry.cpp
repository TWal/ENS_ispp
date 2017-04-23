#include "CompEntry.h"
#include <limits>
#include <cmath>
#include <algorithm>
#include <cassert>

const double INF = std::numeric_limits<double>::infinity();

struct Dyadic {
    //represents num*2^{-logden}
    Dyadic(uint64_t num_, uint64_t logden_) : num(num_), logden(logden_) {}

    //Add 2^{-x}
    Dyadic add(uint64_t x) {
        if(x > logden) {
            //(num * 2^{x-logden} + 1) 2^{-x} = num*2^{-logden} + 2^{-x}
            return Dyadic((num << (x - logden))+1, x);
        } else {
            //(num+2^{logden-x})*2^{-logden} = num*2^{-logden} + 2^{-x}
            return Dyadic(num + (uint64_t(1) << (logden - x)), logden);
        }
    }

    uint64_t optiNbAdd() {
        if(num == 0) {
            return 0;
        }
        int lsb = __builtin_ffsll(num);
        return __builtin_popcountll(((~num) & ((uint64_t(1) << (logden+1)) - 1)) >> lsb);
    }

    uint64_t nbToAdd() {
        return optiNbAdd();
        //number of zeros between the first 1 and logden?
        uint64_t n = num;
        uint64_t maxi = uint64_t(1) << logden;
        uint64_t res = 0;
        uint64_t i = 1;
        while(i <= logden && n != maxi) {
            uint64_t cur = n + (uint64_t(1) << (logden - i));
            if(cur <= maxi) {
                n = cur;
                res += 1;
            } else {
                i += 1;
            }
        }
        if(res != optiNbAdd()) {
            printf("nbToAdd not equal: %lu vs %lu, num=%lu logden = %lu\n", res, optiNbAdd(), num, logden);
        }
        return res;
    }


    //smallest x st sum + 2^{-x} <= 1
    uint64_t minX() {
        //TODO: optimize this?
        uint64_t res = 0;
        while(add(res).gt1()) {
            ++res;
        }
        //if(res != optiMinX()) {
            //printf("minX not equal: %lu vs %lu, num=%lu logden = %lu\n", res, optiMinX(), num, logden);
        //}
        return res;
    }

    bool gt1() {
        return num > (uint64_t(1) << logden);
    }

    bool eq1() {
        return num == (uint64_t(1) << logden);
    }

    uint64_t num;
    uint64_t logden;
};

struct OptiHeightFinder {
    std::vector<Entry> entries;
    std::vector<uint64_t> heights;
    std::vector<uint64_t> best;
    double bestCost;
    uint64_t bestHeightMax;

    OptiHeightFinder(const std::vector<Entry>& entries_) : entries(entries_), heights(entries.size()), best(), bestCost(INF), bestHeightMax(std::numeric_limits<size_t>::max()) {}

    double costLowerBound(size_t i, size_t minH) {
        double res = 0;
        for(size_t j = i; j < entries.size(); ++j) {
            res += entries[j].cost(minH);
        }
        return res;
    }

    void solve(size_t i, double curCost, uint64_t curMaxHeight, Dyadic curSum) {
        if(curSum.gt1()) return;
        if(curSum.eq1() && i != entries.size()) return;
        if(curCost > bestCost) return;
        if(curSum.nbToAdd() > (entries.size() - i)) return;
        if(i == entries.size()) {
            assert(curSum.eq1());
            if(curCost <= bestCost + std::numeric_limits<float>::epsilon()) {
                if(std::fabs(curCost - bestCost) >= std::numeric_limits<float>::epsilon()
                || curMaxHeight < bestHeightMax) {
                    bestCost = curCost;
                    best = heights;
                    bestHeightMax = curMaxHeight;
                }
            }
            return;
        }
        if(curCost + costLowerBound(i, curSum.minX()) > bestCost) return;
        for(uint64_t h = 1; h <= entries.size(); ++h) {
            heights[i] = h;
            solve(i+1, curCost + entries[i].cost(h), std::max(h, curMaxHeight), curSum.add(h));
        }
    }

    void solve() {
        solve(0, 0, 0, Dyadic(0, 0));
    }
};

struct HeightToCode {
    std::vector<std::pair<uint64_t, Entry>> entries;
    std::vector<std::pair<uint32_t, uint32_t>> result; //val, length

    HeightToCode(const std::vector<Entry>& entries_, const std::vector<uint64_t>& heights) {
        assert(entries_.size() == heights.size());
        entries.reserve(entries_.size());
        for(size_t i = 0; i < entries_.size(); ++i) {
            entries.push_back(std::make_pair(heights[i], entries_[i]));
        }
    }

    void solve(uint32_t val, uint32_t length) {
        if(result.size() == entries.size()) return;
        if(entries[result.size()].first == length) {
            result.push_back(std::make_pair(val, length));
            return;
        }
        solve(val, length+1);
        solve(val | (uint32_t(1) << length), length+1);
    }

    void solve() {
        std::sort(entries.begin(), entries.end(), [](const std::pair<uint64_t, Entry>& e1, const std::pair<uint64_t, Entry>& e2) {
            return e1.first < e2.first;
        });
        solve(0, 0);
    }
};

std::vector<CompEntry> optimizeEntries(const std::vector<Entry>& entries) {
    OptiHeightFinder heightFinder(entries);
    heightFinder.solve();
    //printf("Cost:%lf\n", heightFinder.bestCost);
    HeightToCode h2c(entries, heightFinder.best);
    h2c.solve();
    std::vector<CompEntry> result;
    for(size_t i = 0; i < entries.size(); ++i) {
        CompEntry c;
        c.name = h2c.entries[i].second.name;
        c.head_size = h2c.result[i].second;
        c.val = h2c.result[i].first;
        c.nb6Pt = h2c.entries[i].second.nbPt;
        assert(h2c.entries[i].first == h2c.result[i].second);
        result.push_back(c);
    }
    return result;
}

