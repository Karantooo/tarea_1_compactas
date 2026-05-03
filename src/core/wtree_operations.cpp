#include "wtree.hpp"

namespace CDS_UDEC {
  uint32_t WaveletTree::access(uint32_t index) {
    int h = height;
    uint32_t ele = 0;
    Node *aux = root;
    while(aux != nullptr) {
      if(aux -> representation.access(index) == 0) {
        index = aux -> representation.rank0(index);
        aux = aux -> childs[0];
      }
      else {
        index = aux -> representation.rank1(index);
        aux = aux -> childs[1];
        ele |= (1 << h);
      }
      h--;
    }
    return ele;
  }

  uint32_t WaveletTree::rank(uint32_t symbol, uint32_t index) {
    int h = height;
    Node *aux = root;
    while(aux != nullptr) {
      if(symbol & (1 << h)) {
        index = aux -> representation.rank1(index);
        aux = aux -> childs[1];
      }
      else {
        index = aux -> representation.rank0(index);
        aux = aux -> childs[0];
      }
      h--;
    }
    return index;
  }
}
