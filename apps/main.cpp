#include <iostream>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/wavelet_trees.hpp>
#include "greeting.hpp"

int main() {
    sdsl::bit_vector bv = {1,0,1,1,0};
    std::cout << bv[1] << std::endl;

    return 0;
}