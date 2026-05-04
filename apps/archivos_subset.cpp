// mk_subset.cpp
// Usage: ./mk_subset <input_path> <percent>
// Example: ./mk_subset "data/sources" 0.1
// Creates a file next to <input_path> named "<percent> <basename>"
// containing the first floor(percent * filesize) bytes of the input.
// Notes:
//  - Works on arbitrary bytes; does not validate UTF-8. Safe for binary/text.
//  - Percent must be in (0, 1], e.g., 0.1, 0.25, 1.0
//  - Avoids loading the whole file into RAM; streams in chunks.

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cmath>
#include <vector>
#include <cerrno>
#include <cstring>

namespace fs = std::filesystem;

static void copy_prefix_bytes(const fs::path& src, const fs::path& dst, std::uintmax_t want_bytes) {
    std::ifstream in(src, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Cannot open input: " + src.string() + " (" + std::strerror(errno) + ")");
    }
    std::ofstream out(dst, std::ios::binary | std::ios::trunc);
    if (!out) {
        throw std::runtime_error("Cannot create output: " + dst.string() + " (" + std::strerror(errno) + ")");
    }

    const std::size_t BUF = 1 << 20; // 1 MiB buffer
    std::vector<char> buffer(BUF);

    std::uintmax_t remaining = want_bytes;
    while (remaining > 0 && in) {
        std::size_t to_read = static_cast<std::size_t>(std::min<std::uintmax_t>(remaining, BUF));
        in.read(buffer.data(), static_cast<std::streamsize>(to_read));
        std::streamsize got = in.gcount();
        if (got <= 0) break;
        out.write(buffer.data(), got);
        remaining -= static_cast<std::uintmax_t>(got);
    }

    if (!out) {
        throw std::runtime_error("Failed writing to: " + dst.string());
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_path> <percent>\n";
        return 1;
    }
    fs::path input_path = argv[1];
    std::string percent_str = argv[2];

    // Validate percent
    double percent = 0.0;
    try {
        percent = std::stod(percent_str);
    } catch (...) {
        std::cerr << "Invalid percent: " << percent_str << "\n";
        return 2;
    }
    if (!(percent > 0.0 && percent <= 1.0)) {
        std::cerr << "Percent must be in (0, 1], got: " << percent << "\n";
        return 3;
    }

    // Check input
    std::error_code ec;
    if (!fs::exists(input_path, ec) || !fs::is_regular_file(input_path, ec)) {
        std::cerr << "Input file does not exist or is not a regular file: " << input_path << "\n";
        return 4;
    }

    // File size
    std::uintmax_t fsize = fs::file_size(input_path, ec);
    if (ec) {
        std::cerr << "Cannot get file size: " << input_path << "\n";
        return 5;
    }
    if (fsize == 0) {
        std::cerr << "Input is empty: " << input_path << "\n";
        return 6;
    }

    // Compute bytes to copy
    std::uintmax_t want = static_cast<std::uintmax_t>(std::floor(percent * static_cast<long double>(fsize)));
    if (want == 0) {
        // Ensure at least 1 byte if percent > 0
        want = std::min<std::uintmax_t>(1, fsize);
    }
    if (want > fsize) want = fsize;

    // Build destination path: "<percent_str> <basename>" in the same directory
    fs::path parent = input_path.parent_path();
    std::string base = input_path.filename().string();
    fs::path dest = parent / (percent_str + " " + base);

    try {
        copy_prefix_bytes(input_path, dest, want);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 7;
    }

    std::cout << "Created: " << dest << " (" << want << " / " << fsize << " bytes, " 
              << (100.0 * static_cast<long double>(want) / static_cast<long double>(fsize)) << "%)\n";
    return 0;
}
