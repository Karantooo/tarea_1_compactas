#include "bitvector.hpp"

namespace CDS_UDEC {
  void BitVector::add_select1_2(uint8_t _s) {
    add_rank();
    s = std::max(_s, (uint8_t)8);
    uint32_t ss = 1ULL << s;
    std::vector<uint32_t> aux_select;
    for(uint32_t i = 0; i < size; ++i) {
      if(access(i) == 1)
        aux_select.push_back(i);
    }
    uint32_t m = (uint32_t) aux_select.size();
    uint32_t size_s2 = (m + ss - 1) >> s;
    S2.resize(size_s2);
    for(uint32_t p = 0; p < size_s2; ++p) {
      S2[p] = aux_select[p * ss];
    }
    S2.push_back(size);
  }

  uint32_t BitVector::select1_2(uint32_t j) {
    if(j == 0) return size;
    if(j > super_blocks[super_blocks.size() - 1]) return size;

    uint32_t bucket_s = j >> s;
    uint32_t sb_L = S2[bucket_s] >> 9;
    uint32_t sb_R = (S2[bucket_s + 1] + 511) >> 9;
    while(sb_R - sb_L > 1) {
      uint32_t mid = sb_L + ((sb_R - sb_L) >> 1);
      if(super_blocks[mid] < j)
        sb_L = mid;
      else
        sb_R = mid;
    }

    uint32_t qty = super_blocks[sb_L];
    uint32_t indexL = sb_L << 9;
    sb_R = std::min(sb_R << 9, S2[bucket_s + 1]);
    uint32_t indexR = sb_R;
    uint32_t bucket_bL = indexL >> 6;
    uint32_t bucket_bR = (indexR + 63) >> 6;
    for(; bucket_bL < bucket_bR; ++bucket_bL) {
      if(qty + blocks[bucket_bL] >= j)
        break;
    }
    bucket_bL -= 1;
    qty += blocks[bucket_bL];
    uint32_t id = bucket_bL << 6;
    for(uint32_t i = 0; i < 64; ++i) {
      uint32_t ac_val = access(id + i);
      if(qty + ac_val == j)
        return id+i;
      qty+= ac_val;
    }
    return size;
  }
}
