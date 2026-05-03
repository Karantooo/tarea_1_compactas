#include "fm_index.hpp"

FMIndexCount::FMIndexCount(std::array<std::size_t, 257> C,
                           const RankStructure& rank)
    : C_(C), rank_(rank) {}

std::size_t FMIndexCount::count(const std::string& pattern) const {
    if (pattern.empty()) return 0;

    for (char ch : pattern) {
        auto c = static_cast<unsigned char>(ch);
        if (C_[c] == C_[c + 1]) return 0;
    }

    std::size_t i = pattern.size() - 1;
    std::uint32_t v = static_cast<unsigned char>(pattern[i]);

    std::size_t sp = C_[v];
    std::size_t ep = C_[v + 1];

    while (sp < ep && i >= 1) {
        v = static_cast<unsigned char>(pattern[i - 1]);
        sp = C_[v] + rank_.rank(v, sp);
        ep = C_[v] + rank_.rank(v, ep);
        --i;
    }

    return ep > sp ? ep - sp : 0;
}