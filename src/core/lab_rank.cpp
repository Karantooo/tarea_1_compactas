#include "lab_rank.hpp"

LabWaveletRank::LabWaveletRank(std::vector<std::uint32_t> bwt)
    : wt_(bwt) {}

std::size_t LabWaveletRank::rank(std::uint32_t c, std::size_t k) const {
    if (k == 0) return 0;
    return wt_.rank(c, static_cast<std::uint32_t>(k - 1));
}

std::string LabWaveletRank::name() const {
    return "lab_wt";
}