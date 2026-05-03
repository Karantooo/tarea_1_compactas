#pragma once

#include "rank_structure.hpp"
#include <array>
#include <string>

class FMIndexCount {
private:
    std::array<std::size_t, 257> C_;
    const RankStructure& rank_;

public:
    FMIndexCount(std::array<std::size_t, 257> C,
                 const RankStructure& rank);

    std::size_t count(const std::string& pattern) const;
};