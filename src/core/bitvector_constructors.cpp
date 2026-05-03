#include "bitvector.hpp"

namespace CDS_UDEC {
  BitVector::BitVector() {}

  BitVector::BitVector(uint32_t _size, uint8_t _default_value) {
    size = _size;
    uint64_t default_value = _default_value == 0 ? ZERO : ONE;
    uint32_t buckets_needed = size >> 6;  //size / (2^i)
    uint64_t module_value = size & 63; //size % 64;
    if(module_value != ZERO)
      buckets_needed++;
    bitarray.assign(buckets_needed, default_value);
  }
}