#pragma once

#include "rank_structure.hpp"
#include <vector>

class BruteRank final : public RankStructure {
private:
    std::vector<std::uint32_t> bwt_;

public:
    explicit BruteRank(std::vector<std::uint32_t> bwt);

    std::size_t rank(std::uint32_t c, std::size_t k) const override;
    std::string name() const override;
};