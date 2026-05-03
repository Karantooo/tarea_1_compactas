#include "bitvector.hpp"

namespace CDS_UDEC {
  void BitVector::add_rank() {
    //superblock with size 512
    uint64_t needed_sb = ((size + 511) >> 9) + 1ULL;
    super_blocks.reserve(needed_sb);
    //block with size 64
    uint64_t needed_b = ((size + 63) >> 6) + 1ULL;
    blocks.reserve(needed_b);

    uint32_t ps_sb = 0;
    uint16_t ps_b = 0;
    uint64_t c_bits_sb = 0;
    uint64_t c_bits_b = 0;
    super_blocks.push_back(0ULL);
    blocks.push_back(0ULL);
    for(uint64_t const &decimal: bitarray) {
      c_bits_sb += 64;
      c_bits_b += 64;
      uint32_t popcount_decimal = __builtin_popcountll(decimal);
      ps_sb += popcount_decimal;
      ps_b += popcount_decimal;
      if(!(c_bits_sb & 511)) {
        super_blocks.push_back(ps_sb);
        ps_b = 0ULL;
      }
      if(!(c_bits_b & 63)) {
        blocks.push_back(ps_b);
        c_bits_b = 0;
      }
    }
    super_blocks.push_back(ps_sb);
    blocks.push_back(0ULL);
  }

  uint32_t BitVector::rank1(uint32_t index) {
    uint64_t super_block = index >> 9;
    uint64_t block = index >> 6;
    uint32_t bucket = index >> 6;
    uint64_t bit_index = index & 63;
    uint64_t inverted_bit_index = 63 ^ bit_index;
    uint64_t ibi_p1 = inverted_bit_index + 1ULL;
    uint32_t popcount = ibi_p1 == 64 ? 0ULL : __builtin_popcountll(bitarray[bucket] >> ibi_p1);
    return super_blocks[super_block] + blocks[block] + popcount;
  }

  uint32_t BitVector::rank0(uint32_t index) {
    return index - rank1(index);
  }
  
  uint32_t BitVector::rank1_lineal(uint32_t index) {
    uint32_t count = 0;
    for(uint32_t i = 0; i < index; ++i) {
      if(access(i) == 1)
        count++;
    }
    return count;
  }

  uint32_t BitVector::rank0_lineal(uint32_t index) {
    return index - rank1_lineal(index);
  }
}
