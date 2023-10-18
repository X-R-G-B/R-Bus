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
                static_assert(std::is_pod<T>::value, "ZSTD: Data must be POD");
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
                size_t decompressedSize = ZSTD_decompress(decompressedArray.data(), MAX_PACKET_SIZE, data.data(), size);

                if (ZSTD_isError(decompressedSize)) {
                    throw std::runtime_error(std::string("ZSTD: Error while decompressing: ") + ZSTD_getErrorName(decompressedSize));
                }
                return decompressedArray;
            }

            static std::size_t getFrameContentSize(const std::array<char, MAX_PACKET_SIZE> &data)
            {
                return ZSTD_getFrameContentSize(data.data(), data.size());
            }
    };
} // namespace Nitwork
