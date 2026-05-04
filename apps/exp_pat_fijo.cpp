// src/fm_experimento_1.cpp
//
// Uso:
//   ./fm_experimento_1 <text_path> <pattern> <kind>
//
// kind:
//   sdsl_huff_rrr   -> csa_wt<wt_huff<rrr_vector<127>>, 512, 1024>
//   sdsl_blcd       -> csa_wt<wt_blcd<>, 512, 1024>
//   lab_wt          -> WaveletTree del laboratorio sobre BWT explícita
//   brute           -> rank lineal sobre BWT explícita
//
// Salida:
//   kind;text_path;pattern;count;time_ns
//
// Nota:
//   Este archivo se enfoca en count(T, P). Para el informe experimental,
//   después puedes volver a agregar medición de tiempo y espacio.

#include <sdsl/suffix_arrays.hpp>
#include <sdsl/wavelet_trees.hpp>

#include "wtree.hpp"
#include "fm_index_wavelet.hpp"
#include "utils.hpp"

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace sdsl;

// ============================================================
// 1. Tipos SDSL a comparar
// ============================================================

using fm_huff_rrr_t = csa_wt<wt_huff<rrr_vector<127>>, 512, 1024>;
using fm_blcd_t     = csa_wt<wt_blcd<>, 512, 1024>;

// ============================================================
// 2. Lectura y saneo del texto
// ============================================================

string read_text_file_sanitized(const string& text_path) {
    ifstream in(text_path, ios::binary);
    if (!in) {
        throw runtime_error("No se pudo abrir el archivo: " + text_path);
    }

    string text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    if (text.empty()) {
        throw runtime_error("Archivo vacío: " + text_path);
    }

    // Saneamos 0x00 porque usaremos 0 como centinela '$'.
    bool had_nuls = false;
    for (char& ch : text) {
        if (ch == '\0') {
            ch = static_cast<char>(1);
            had_nuls = true;
        }
    }

    if (had_nuls) {
        cerr << "[WARN] Se encontraron bytes 0x00 y se mapearon a 0x01.\n";
    }

    return text;
}

// ============================================================
// 3. Construcción de BWT explícita
//    Basada en el código entregado en el enunciado/laboratorio,
//    pero adaptada para trabajar desde string ya saneado.
// ============================================================

vector<uint32_t> build_bwt_from_text(const string& text) {
    const size_t n_text = text.size();
    const size_t n = n_text + 1;  // texto + centinela 0

    int_vector<> seq(n, 0, 8);

    for (size_t i = 0; i < n_text; ++i) {
        seq[i] = static_cast<unsigned char>(text[i]);
    }
    seq[n - 1] = 0;  // centinela '$'

    int_vector<> sa(n, 0, bits::hi(n) + 1);

    algorithm::calculate_sa(
        reinterpret_cast<const unsigned char*>(seq.data()),
        static_cast<int32_t>(n),
        sa
    );

    vector<uint32_t> bwt(n);

    for (size_t i = 0; i < n; ++i) {
        if (sa[i] == 0) {
            bwt[i] = seq[n - 1];
        } else {
            bwt[i] = seq[sa[i] - 1];
        }
    }

    return bwt;
}

// C[c] = cantidad de símbolos en T$ estrictamente menores que c.
array<size_t, 257> build_C_table(const string& text) {
    array<size_t, 257> freq{};
    array<size_t, 257> C{};

    // Centinela '$' = 0.
    freq[0] = 1;

    for (unsigned char c : text) {
        freq[c]++;
    }

    size_t acc = 0;
    for (size_t c = 0; c < 256; ++c) {
        C[c] = acc;
        acc += freq[c];
    }
    C[256] = acc;

    return C;
}



// ============================================================
// 9. Factory de implementaciones
// ============================================================

unique_ptr<RankStructure> make_rank_structure(
    const string& kind,
    const string& text,
    const string& text_path
) {
    if (kind == "sdsl_huff_rrr") {
        const string index_path = text_path + ".sdsl_huff_rrr.fm";
        return make_unique<SdslRank<fm_huff_rrr_t>>(
            text,
            index_path,
            "sdsl_huff_rrr"
        );
    }

    if (kind == "sdsl_blcd") {
        const string index_path = text_path + ".sdsl_blcd.fm";
        return make_unique<SdslRank<fm_blcd_t>>(
            text,
            index_path,
            "sdsl_blcd"
        );
    }

    if (kind == "lab_wt") {
        vector<uint32_t> bwt = build_bwt_from_text(text);
        return make_unique<LabWaveletRank>(std::move(bwt));
    }

    if (kind == "brute") {
        vector<uint32_t> bwt = build_bwt_from_text(text);
        return make_unique<BruteRank>(std::move(bwt));
    }

    throw invalid_argument(
        "Implementación desconocida: " + kind +
        "\nOpciones: sdsl_huff_rrr, sdsl_blcd, lab_wt, brute"
    );
}

// ============================================================
// 10. main
// ============================================================

int main(int argc, char** argv) {
    if (argc < 4) {
        cerr << "Uso:\n"
             << "  " << argv[0] << " <text_path> <pattern> <kind>\n\n"
             << "kind:\n"
             << "  sdsl_huff_rrr\n"
             << "  sdsl_blcd\n"
             << "  lab_wt\n"
             << "  brute\n";
        return 2;
    }

    try {
        const string text_path = argv[1];
        const string pattern = argv[2];
        const string kind = argv[3];

        const string text = read_text_file_sanitized(text_path);
        const auto C = build_C_table(text);

        unique_ptr<RankStructure> rank_structure =
            make_rank_structure(kind, text, text_path);

        FMIndexCount fm_count(C, *rank_structure);

        double t_mean = 0;
        size_t N_RUNS = 32;
        size_t occs;

        for (size_t i = 0; i < N_RUNS; i++) {
            const auto t_start = chrono::high_resolution_clock::now();
            occs = fm_count.count(pattern);
            const auto t_end = chrono::high_resolution_clock::now();
            const auto elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(t_end - t_start).count();
            t_mean += (double)elapsed_ns;
        }

        t_mean /= (double)N_RUNS;

        cout << rank_structure->name() << ';'
             << text_path << ';'
             << pattern << ';'
             << occs << ';'
             << t_mean; // << '\n';

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << '\n';
        return 1;
    }

    return 0;
}
