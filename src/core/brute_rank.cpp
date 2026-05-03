#include "brute_rank.hpp"

BruteRank::BruteRank(std::vector<std::uint32_t> bwt)
    : bwt_(std::move(bwt)) {}

std::size_t BruteRank::rank(std::uint32_t c, std::size_t k) const {
    if (k > bwt_.size()) k = bwt_.size();

    std::size_t cnt = 0;
    for (std::size_t i = 0; i < k; ++i) {
        if (bwt_[i] == c) cnt++;
    }
    return cnt;
}

std::string BruteRank::name() const {
    return "brute";
}