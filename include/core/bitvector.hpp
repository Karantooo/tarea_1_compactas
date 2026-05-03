#pragma once
#include <vector>
#include <cstdint>

uint64_t const ZERO = 0x0000000000000000ULL;
uint64_t const ONE  = 0xFFFFFFFFFFFFFFFFULL;

namespace CDS_UDEC {
  class BitVector
  {
  private:
    std::vector<uint64_t> bitarray;
    std::vector<uint16_t> blocks;
    std::vector<uint32_t> super_blocks;
    std::vector<uint32_t> S1;
    std::vector<uint32_t> S2;
    uint8_t s;
    uint32_t size;
  public:
    BitVector();
    BitVector(uint32_t _size, uint8_t _default_value);
    uint64_t access(uint32_t index);
    void flip(uint32_t index);
    void set(uint32_t index);
    void add_rank();
    uint32_t rank1(uint32_t index);
    uint32_t rank0(uint32_t index);
    uint32_t rank1_lineal(uint32_t j);
    uint32_t rank0_lineal(uint32_t j);
    void add_select1_1();
    uint32_t select1_1(uint32_t j);
    void add_select1_2(uint8_t _s = 7);
    uint32_t select1_2(uint32_t j);
    
  };
}
