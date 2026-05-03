#include "wtree.hpp"

namespace CDS_UDEC {
  WaveletTree::WaveletTree(std::vector<uint32_t> &sequence) {
    sigma = 0;
    for(auto const &e: sequence) {
      sigma = std::max(e, sigma);
    }
    size = (uint32_t) sequence.size();
    height = std::floor(std::log2(sigma));
    root = build(sequence, 0, size, height);
  } 

  WaveletTree::Node* WaveletTree::build(std::vector<uint32_t> const &sequence, uint32_t l, uint32_t r, int h) {
    if((uint32_t)sequence.size() == 0)
      return nullptr;
    if(h == -1)
      return nullptr;
    std::vector<uint32_t> left_side, right_side;
    WaveletTree::Node *node = new WaveletTree::Node(r - l);
    for(uint32_t i = 0; i < r; ++i) {
      uint32_t ele = sequence[i];
      if(ele & (1 << h)) {
        right_side.push_back(ele);
        node -> representation.set(i);
      }
      else
        left_side.push_back(ele);
    }
    node -> representation.add_rank();
    node -> childs[0] = build(left_side, 0, (uint32_t) left_side.size(), h-1);
    node -> childs[1] = build(right_side, 0, (uint32_t) right_side.size(), h-1);
    return node;
  }
}
