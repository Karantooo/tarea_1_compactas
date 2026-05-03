#pragma once

#include "rank_structure.hpp"
#include "wtree.hpp"
#include <vector>

class LabWaveletRank final : public RankStructure {
private:
    mutable CDS_UDEC::WaveletTree wt_;

public:
    explicit LabWaveletRank(std::vector<std::uint32_t> bwt);

    std::size_t rank(std::uint32_t c, std::size_t k) const override;
    std::string name() const override;
};