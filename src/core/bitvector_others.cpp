#include "bitvector.hpp"

namespace CDS_UDEC {
  uint64_t BitVector::access(uint32_t index) {
    uint32_t bucket = index >> 6;
    uint64_t bit_index = index & 63;
    uint64_t inverted_bit_index = 63 ^ bit_index;
    return (bitarray[bucket] >> inverted_bit_index) & 1;
  }

  void BitVector::flip(uint32_t index) {
    uint32_t bucket = index >> 6;
    uint64_t bit_index = index & 63;
    uint64_t inverted_bit_index = 63 ^ bit_index;
    bitarray[bucket] ^= (1ULL << inverted_bit_index);
  }

  void BitVector::set(uint32_t index) {
    uint32_t bucket = index >> 6;
    uint64_t bit_index = index & 63;
    uint64_t inverted_bit_index = 63 ^ bit_index;
    bitarray[bucket] |= (1ULL << inverted_bit_index);
  }
}
