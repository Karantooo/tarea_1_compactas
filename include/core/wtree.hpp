#pragma once
#include "bitvector-lib/bitvector.hpp"
#include <cmath>
namespace CDS_UDEC {
  class WaveletTree
  {
    private:
      struct Node {
        CDS_UDEC::BitVector representation;
        Node *childs[2];
        Node() = default;
        Node(uint32_t len) {
          representation = CDS_UDEC::BitVector(len + 1, 0);
          childs[0] = nullptr, childs[1] = nullptr;
        }
      };
      Node* build(std::vector<uint32_t> const &sequence, uint32_t l, uint32_t r, int h);
      Node *root;
      uint32_t sigma;
      uint32_t size;
      uint32_t height;
    public:
      WaveletTree() = default;
      WaveletTree(std::vector<uint32_t> &sequence);
      uint32_t access(uint32_t index);
      uint32_t rank(uint32_t symbol, uint32_t index);
  };
}
