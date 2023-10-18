/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** zstd
*/

#include <string>
#include <array>
#include <stdexcept>
#include <zstd.h>

namespace Nitwork {
    constexpr int MAX_PACKET_SIZE = 1024;
    constexpr int COMPRESSION_LEVEL = 1;

    class Zstd {
        public:
            template <typename T>
            static std::array<char, MAX_PACKET_SIZE> compress(const T &data)
            {
                std::array<char, MAX_PACKET_SIZE> compressed = {};
                size_t compressedSize = ZSTD_compress(compressed.data(), MAX_PACKET_SIZE, &data, sizeof(T), COMPRESSION_LEVEL);

                if (ZSTD_isError(compressedSize)) {
                    throw std::runtime_error("ZSTD: Error while compressing");
                }
                return compressed;
            }

            static std::array<char, MAX_PACKET_SIZE> decompress(const std::array<char, MAX_PACKET_SIZE> &data)
            {
                std::array<char, MAX_PACKET_SIZE> decompressed = {};
                size_t decompressedSize = ZSTD_decompress(decompressed.data(), MAX_PACKET_SIZE, data.data(), data.size());

                if (ZSTD_isError(decompressedSize)) {
                    throw std::runtime_error("ZSTD: Error while decompressing");
                }
                return decompressed;
            }
    };
} // namespace Nitwork
