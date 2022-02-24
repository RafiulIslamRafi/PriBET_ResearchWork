#pragma once

#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <type_traits>
#include <limits>
#include "seal/util/defines.h"

namespace seal
{
    namespace util
    {
        constexpr int bytes_per_uint64 = sizeof(std::uint64_t);

        constexpr int bytes_per_uint32 = sizeof(std::uint32_t);

        constexpr int uint32_per_uint64 = 2;

        constexpr int bits_per_nibble = 4;

        constexpr int bits_per_byte = 8;

        constexpr int bits_per_uint64 = bytes_per_uint64 * bits_per_byte;

        constexpr int bits_per_uint32 = bytes_per_uint32 * bits_per_byte;

        constexpr int nibbles_per_byte = 2;

        constexpr int nibbles_per_uint64 = bytes_per_uint64 * nibbles_per_byte;

        constexpr std::uint64_t uint64_high_bit = 1ULL << (bits_per_uint64 - 1);

        inline constexpr std::uint32_t reverse_bits(std::uint32_t operand) noexcept
        {
            operand = (((operand & 0xaaaaaaaa) >> 1) | ((operand & 0x55555555) << 1));
            operand = (((operand & 0xcccccccc) >> 2) | ((operand & 0x33333333) << 2));
            operand = (((operand & 0xf0f0f0f0) >> 4) | ((operand & 0x0f0f0f0f) << 4));
            operand = (((operand & 0xff00ff00) >> 8) | ((operand & 0x00ff00ff) << 8));
            return((operand >> 16) | (operand << 16));
        }

        template<typename T, typename = is_uint64_t<T>>
        inline constexpr T reverse_bits(T operand) noexcept
        {
            return static_cast<T>(reverse_bits(static_cast<std::uint32_t>(operand >> 32))) |
                (static_cast<T>(reverse_bits(static_cast<std::uint32_t>(operand & 0xFFFFFFFFULL))) << 32);
        }

        inline std::uint32_t reverse_bits(std::uint32_t operand, int bit_count)
        {
#ifdef SEAL_DEBUG
            if (bit_count < 0 || bit_count > 32)
            {
                throw std::invalid_argument("bit_count");
            }
#endif
            // We need shift by 32 to return zero so convert to uint64_t in-between
            return static_cast<std::uint32_t>(
                (static_cast<std::uint64_t>(reverse_bits(operand)) >> (32 - bit_count)));
        }

        template<typename T, typename = is_uint64_t<T>>
        inline T reverse_bits(T operand, int bit_count)
        {
#ifdef SEAL_DEBUG
            if (bit_count < 0 || bit_count > 64)
            {
                throw std::invalid_argument("bit_count");
            }
#endif
            // Need return zero on shift by 64
            return (bit_count == 0) ? 0 : (reverse_bits(operand) >> (64 - bit_count));
        }

        inline void get_msb_index_generic(unsigned long *result, std::uint64_t value)
        {
#ifdef SEAL_DEBUG
            if (result == nullptr)
            {
                throw std::invalid_argument("result");
            }
#endif
            static const unsigned long deBruijnTable64[64] = {
                63,  0, 58,  1, 59, 47, 53,  2,
                60, 39, 48, 27, 54, 33, 42,  3,
                61, 51, 37, 40, 49, 18, 28, 20,
                55, 30, 34, 11, 43, 14, 22,  4,
                62, 57, 46, 52, 38, 26, 32, 41,
                50, 36, 17, 19, 29, 10, 13, 21,
                56, 45, 25, 31, 35, 16,  9, 12,
                44, 24, 15,  8, 23,  7,  6,  5
            };

            value |= value >> 1;
            value |= value >> 2;
            value |= value >> 4;
            value |= value >> 8;
            value |= value >> 16;
            value |= value >> 32;

            *result = deBruijnTable64[((value - (value >> 1)) * 0x07EDD5E59A4E28C2ULL) >> 58];
        }

        inline int get_significant_bit_count(std::uint64_t value)
        {
            if (value == 0)
            {
                return 0;
            }

            unsigned long result;
            SEAL_MSB_INDEX_UINT64(&result, value);
            return static_cast<int>(result + 1);
        }

        inline bool is_hex_char(char hex)
        {
            if (hex >= '0' && hex <= '9')
            {
                return true;
            }
            if (hex >= 'A' && hex <= 'F')
            {
                return true;
            }
            if (hex >= 'a' && hex <= 'f')
            {
                return true;
            }
            return false;
        }

        inline char nibble_to_upper_hex(int nibble)
        {
#ifdef SEAL_DEBUG
            if (nibble < 0 || nibble > 15)
            {
                throw std::invalid_argument("nibble");
            }
#endif
            if (nibble < 10)
            {
                return static_cast<char>(nibble + static_cast<int>('0'));
            }
            return static_cast<char>(nibble + static_cast<int>('A') - 10);
        }

        inline int hex_to_nibble(char hex)
        {
            if (hex >= '0' && hex <= '9')
            {
                return static_cast<int>(hex) - static_cast<int>('0');
            }
            if (hex >= 'A' && hex <= 'F')
            {
                return static_cast<int>(hex) - static_cast<int>('A') + 10;
            }
            if (hex >= 'a' && hex <= 'f')
            {
                return static_cast<int>(hex) - static_cast<int>('a') + 10;
            }
#ifdef SEAL_DEBUG
            throw std::invalid_argument("hex");
#endif
            return -1;
        }

        inline SEAL_BYTE *get_uint64_byte(std::uint64_t *value, std::size_t byte_index)
        {
#ifdef SEAL_DEBUG
            if (value == nullptr)
            {
                throw std::invalid_argument("value");
            }
#endif
            return reinterpret_cast<SEAL_BYTE*>(value) + byte_index;
        }

        inline const SEAL_BYTE *get_uint64_byte(const std::uint64_t *value, std::size_t byte_index) 
        {
#ifdef SEAL_DEBUG
            if (value == nullptr)
            {
                throw std::invalid_argument("value");
            }
#endif
            return reinterpret_cast<const SEAL_BYTE*>(value) + byte_index;
        }

        inline int get_hex_string_bit_count(const char *hex_string, int char_count)
        {
#ifdef SEAL_DEBUG
            if (hex_string == nullptr && char_count > 0)
            {
                throw std::invalid_argument("hex_string");
            }
            if (char_count < 0)
            {
                throw std::invalid_argument("char_count");
            }
#endif
            for (int i = 0; i < char_count; i++)
            {
                char hex = *hex_string++;
                int nibble = hex_to_nibble(hex);
                if (nibble != 0)
                {
                    int nibble_bits = get_significant_bit_count(static_cast<std::uint64_t>(nibble));
                    int remaining_nibbles = (char_count - i - 1) * bits_per_nibble;
                    return nibble_bits + remaining_nibbles;
                }
            }
            return 0;
        }

        template<typename T, typename = std::enable_if<std::is_integral<T>::value>>
        inline T divide_round_up(T value, T divisor)
        {
#ifdef SEAL_DEBUG
            if (value < 0)
            {
                throw std::invalid_argument("value");
            }
            if (divisor <= 0)
            {
                throw std::invalid_argument("divisor");
            }
#endif
            return (value + divisor - 1) / divisor;
        }
        
        template<typename T>
        constexpr double epsilon = std::numeric_limits<T>::epsilon();

        template<typename T,
            typename = std::enable_if_t<std::is_floating_point<T>::value>>
        constexpr bool are_close(T value1, T value2) noexcept
        {
            return std::fabs(value1 - value2) < epsilon<T>;
        }

        template<typename T, 
            typename = std::enable_if_t<std::is_integral<T>::value>>
        constexpr bool is_zero(T value) noexcept
        {
            return value == T{ 0 };
        }
    }
}
