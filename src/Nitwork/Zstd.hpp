/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** zstd
*/

#include <array>
#include <stdexcept>
#include <string>
#include <zstd.h>

namespace Nitwork {
    constexpr int MAX_PACKET_SIZE   = 1024;
    constexpr int COMPRESSION_LEVEL = 1;

    class Zstd {
        public:
            template <typename T>
            static std::vector<char> compress(const T &data)
            {
                if (!std::is_standard_layout_v<T> || !std::is_trivial_v<T>) {
                    throw std::runtime_error("ZSTD: Data must be POD");
                }
                size_t const compressedSize = ZSTD_compressBound(sizeof(T));
                std::vector<char> compressedData(compressedSize);
                size_t const result = ZSTD_compress(
                    compressedData.data(),
                    compressedSize,
                    reinterpret_cast<const char *>(&data),
                    sizeof(T),
                    COMPRESSION_LEVEL);

                if (ZSTD_isError(result)) {
                    throw std::runtime_error(
                        std::string("ZSTD: Error while compressing: ") + ZSTD_getErrorName(result));
                }
                compressedData.resize(result);
                return compressedData;
            }

            static std::array<char, MAX_PACKET_SIZE>
            decompress(const std::array<char, MAX_PACKET_SIZE> &data, size_t size)
            {
                std::array<char, MAX_PACKET_SIZE> decompressedArray = {0};
                size_t decompressedSize =
                    ZSTD_decompress(decompressedArray.data(), MAX_PACKET_SIZE, data.data(), size);

                if (ZSTD_isError(decompressedSize) != 0U) {
                    throw std::runtime_error(
                        std::string("ZSTD: Error while decompressing: ")
                        + ZSTD_getErrorName(decompressedSize));
                }
                if (decompressedSize > MAX_PACKET_SIZE) {
                    throw std::runtime_error("ZSTD: Decompressed size is too big");
                }
                return decompressedArray;
            }

            static std::size_t getFrameContentSize(const std::array<char, MAX_PACKET_SIZE> &data)
            {
                std::size_t const frameContentSize = ZSTD_getFrameContentSize(data.data(), data.size());

                if (ZSTD_isError(frameContentSize) != 0U) {
                    throw std::runtime_error(
                        std::string("ZSTD: Error while getting frame content size: ")
                        + ZSTD_getErrorName(frameContentSize));
                }
                return frameContentSize;
            }
    };
} // namespace Nitwork
