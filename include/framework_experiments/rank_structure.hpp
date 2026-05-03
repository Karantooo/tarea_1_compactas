#pragma once
#include <cstdint>
#include <string>

class RankStructure {
public:
    virtual ~RankStructure() = default;

    virtual std::size_t rank(std::uint32_t c, std::size_t k) const = 0;
    virtual std::string name() const = 0;
};