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
            static std::vector<char> compress(const T &data)
            {
                Logger::debug("ZSTD: Compressing data");
                size_t const compressedSize = ZSTD_compressBound(sizeof(T));
                std::vector<char> compressedData(compressedSize);
                size_t const result = ZSTD_compress(
                    compressedData.data(),
                    compressedSize,
                    reinterpret_cast<const char *>(&data),
                    sizeof(T),
                    COMPRESSION_LEVEL);

                if (ZSTD_isError(result)) {
                    throw std::runtime_error("ZSTD: Error while compressing");
                }
                compressedData.resize(result);
                Logger::debug("ZSTD: Compressed data");
                return compressedData;
            }

            static std::array<char, MAX_PACKET_SIZE> decompress(const std::array<char, MAX_PACKET_SIZE> &data, size_t size)
            {
                std::vector<char> decompressed(MAX_PACKET_SIZE);
                size_t decompressedSize = ZSTD_decompress(decompressed.data(), MAX_PACKET_SIZE, data.data(), size);

                if (ZSTD_isError(decompressedSize)) {
                    throw std::runtime_error("ZSTD: Error while decompressing");
                }
                decompressed.resize(decompressedSize);
                std::array<char, MAX_PACKET_SIZE> decompressedArray;
                std::copy(decompressed.begin(), decompressed.end(), decompressedArray.begin());
                return decompressedArray;
            }

            static std::size_t getFrameContentSize(const std::array<char, MAX_PACKET_SIZE> &data)
            {
                return ZSTD_getFrameContentSize(data.data(), data.size());
            }
    };
} // namespace Nitwork
