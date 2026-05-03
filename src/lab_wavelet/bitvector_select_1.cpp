#include "bitvector.hpp"

namespace CDS_UDEC
{
  void BitVector::add_select1_1() {
    for(uint32_t i = 0; i < size; ++i) {
      if(access(i) == 1)
        S1.push_back(i);
    }
  }

  uint32_t BitVector::select1_1(uint32_t j) {
    if(j == 0) return size;
    if(j > (uint32_t)S1.size()) return size;
    return S1[j-1];
  }
}
