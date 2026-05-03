#pragma once

#include "rank_structure.hpp"
#include <sdsl/suffix_arrays.hpp>
#include <filesystem>
#include <iostream>
#include <stdexcept>

template <typename FMIndexType>
class SdslRank final : public RankStructure {
private:
    FMIndexType fm_index_;
    std::string name_;

public:
    SdslRank(const std::string& text,
             const std::string& index_path,
             std::string name)
        : name_(std::move(name)) {

        if (std::filesystem::exists(index_path)) {
            std::cerr << "[INFO] Cargando índice: " << index_path << '\n';
            sdsl::load_from_file(fm_index_, index_path);
        } else {
            std::cerr << "[INFO] Construyendo índice\n";
            sdsl::construct_im(fm_index_, text, 1);
            sdsl::store_to_file(fm_index_, index_path);
        }
    }

    std::size_t rank(std::uint32_t c, std::size_t k) const override {
        return fm_index_.wavelet_tree.rank(k, static_cast<unsigned char>(c));
    }

    std::string name() const override {
        return name_;
    }
};