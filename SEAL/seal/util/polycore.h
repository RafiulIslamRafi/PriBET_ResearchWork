#pragma once

#include <cstdint>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <limits>
#include "seal/util/common.h"
#include "seal/util/uintcore.h"
#include "seal/util/pointer.h"

namespace seal
{
    namespace util
    {
        inline std::string poly_to_hex_string(const std::uint64_t *value, 
            std::size_t coeff_count, std::size_t coeff_uint64_count)
        {
#ifdef SEAL_DEBUG
            if (coeff_uint64_count > 0 && coeff_count > 0 && value == nullptr)
            {
                throw std::invalid_argument("value");
            }
#endif
            std::ostringstream result;
            bool empty = true;
            value += (coeff_count - 1) * coeff_uint64_count;
            while (coeff_count--)
            {
                if (is_zero_uint(value, coeff_uint64_count))
                {
                    value -= coeff_uint64_count;
                    continue;
                }
                if (!empty)
                {
                    result << " + ";
                }
                result << uint_to_hex_string(value, coeff_uint64_count);
                if (coeff_count > 0)
                {
                    result << "x^" << coeff_count;
                }
                empty = false;
                value -= coeff_uint64_count;
            }
            if (empty)
            {
                result << "0";
            }
            return result.str();
        }

        inline std::string poly_to_dec_string(const std::uint64_t *value, 
            std::size_t coeff_count, std::size_t coeff_uint64_count, MemoryPool &pool)
        {
#ifdef SEAL_DEBUG
            if (coeff_uint64_count > 0 && coeff_count > 0 && value == nullptr)
            {
                throw std::invalid_argument("value");
        }
#endif
            std::ostringstream result;
            bool empty = true;
            value += coeff_count - 1;
            while (coeff_count--)
            {
                if (is_zero_uint(value, coeff_uint64_count))
                {
                    value -= coeff_uint64_count;
                    continue;
                }
                if (!empty)
                {
                    result << " + ";
                }
                result << uint_to_dec_string(value, coeff_uint64_count, pool);
                if (coeff_count > 0)
                {
                    result << "x^" << coeff_count;
                }
                empty = false;
                value -= coeff_uint64_count;
            }
            if (empty)
            {
                result << "0";
            }
            return result.str();
        }

        inline auto allocate_poly(std::size_t coeff_count, 
            std::size_t coeff_uint64_count, MemoryPool &pool)
        {
            return allocate_uint(coeff_count * coeff_uint64_count, pool);
        }

        inline void set_zero_poly(std::size_t coeff_count, 
            std::size_t coeff_uint64_count, std::uint64_t* result)
        {
#ifdef SEAL_DEBUG
            if (result == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("result");
            }
#endif
            set_zero_uint(coeff_count * coeff_uint64_count, result);
        }

        inline auto allocate_zero_poly(std::size_t coeff_count, std::size_t coeff_uint64_count, 
            MemoryPool &pool)
        {
            return allocate_zero_uint(coeff_count * coeff_uint64_count, pool);
        }

        inline std::uint64_t *get_poly_coeff(std::uint64_t *poly, 
            std::size_t coeff_index, std::size_t coeff_uint64_count)
        {
#ifdef SEAL_DEBUG
            if (poly == nullptr)
            {
                throw std::invalid_argument("poly");
            }
#endif
            return poly + coeff_index * coeff_uint64_count;
        }

        inline const std::uint64_t *get_poly_coeff(const std::uint64_t *poly, 
            std::size_t coeff_index, std::size_t coeff_uint64_count)
        {
#ifdef SEAL_DEBUG
            if (poly == nullptr)
            {
                throw std::invalid_argument("poly");
            }
#endif
            return poly + (coeff_index * coeff_uint64_count);
        }

        inline void set_poly_poly(const std::uint64_t *poly, std::size_t coeff_count, 
            std::size_t coeff_uint64_count, std::uint64_t *result)
        {
#ifdef SEAL_DEBUG
            if (poly == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("poly");
            }
            if (result == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("result");
            }
#endif
            set_uint_uint(poly, coeff_count * coeff_uint64_count, result);
        }

        inline bool is_zero_poly(const std::uint64_t *poly, 
            std::size_t coeff_count, std::size_t coeff_uint64_count)
        {
#ifdef SEAL_DEBUG
            if (poly == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("poly");
            }
#endif
            return is_zero_uint(poly, coeff_count * coeff_uint64_count);
        }

        inline bool is_equal_poly_poly(const std::uint64_t *operand1, 
            const std::uint64_t *operand2, std::size_t coeff_count, 
            std::size_t coeff_uint64_count)
        {
#ifdef SEAL_DEBUG
            if (operand1 == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("operand1");
            }
            if (operand2 == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("operand2");
            }
#endif
            return is_equal_uint_uint(operand1, operand2, coeff_count * coeff_uint64_count);
        }

        inline void set_poly_poly(const std::uint64_t *poly, std::size_t poly_coeff_count, 
            std::size_t poly_coeff_uint64_count, std::size_t result_coeff_count, 
            std::size_t result_coeff_uint64_count, std::uint64_t *result)
        {
#ifdef SEAL_DEBUG
            if (poly == nullptr && poly_coeff_count > 0 && poly_coeff_uint64_count > 0)
            {
                throw std::invalid_argument("poly");
            }
            if (result == nullptr && result_coeff_count > 0 && result_coeff_uint64_count > 0)
            {
                throw std::invalid_argument("result");
            }
#endif
            if (!result_coeff_uint64_count || !result_coeff_count)
            {
                return;
            }

            std::size_t min_coeff_count = std::min(poly_coeff_count, result_coeff_count);
            for (std::size_t i = 0; i < min_coeff_count; i++,
                poly += poly_coeff_uint64_count, result += result_coeff_uint64_count)
            {
                set_uint_uint(poly, poly_coeff_uint64_count, result_coeff_uint64_count, result);
            }
            set_zero_uint((result_coeff_count - min_coeff_count) * result_coeff_uint64_count, result);
        }

        inline void set_bigpolyarray_bigpolyarray(const std::uint64_t *operand, 
            std::size_t size, std::size_t coeff_count, std::size_t coeff_uint64_count, 
            std::size_t result_size, std::size_t result_coeff_count, 
            std::size_t result_coeff_uint64_count, std::uint64_t *result)
        {
#ifdef SEAL_DEBUG
            if (operand == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("operand");
            }
            if (result == nullptr && result_coeff_count > 0 && 
                result_coeff_uint64_count > 0 && result_size > 0)
            {
                throw std::invalid_argument("result");
            }
#endif
            std::size_t operand_ptr_increment = coeff_count * coeff_uint64_count;
            std::size_t result_ptr_increment = result_coeff_count * result_coeff_uint64_count;

            std::size_t min_size = std::min(size, result_size);
            for (std::size_t i = 0; i < min_size; i++)
            {
                set_poly_poly(operand, coeff_count, coeff_uint64_count, result_coeff_count, 
                    result_coeff_uint64_count, result);
                operand += operand_ptr_increment;
                result += result_ptr_increment;
            }

            // Set the rest of the polys to zero
            for (std::size_t i = min_size; i < result_size; i++)
            {
                set_zero_uint(result_coeff_count * result_coeff_uint64_count, result);
                result += result_ptr_increment;
            }
        }
        
        inline bool is_one_zero_one_poly(const std::uint64_t *poly, 
            std::size_t coeff_count, std::size_t coeff_uint64_count)
        {
#ifdef SEAL_DEBUG
            if (poly == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("poly");
            }
#endif
            if (coeff_count == 0 || coeff_uint64_count == 0)
            {
                return false;
            }
            if (!is_equal_uint(get_poly_coeff(poly, 0, coeff_uint64_count), 
                coeff_uint64_count, 1))
            {
                return false;
            }
            if (!is_equal_uint(get_poly_coeff(poly, coeff_count - 1, coeff_uint64_count), 
                coeff_uint64_count, 1))
            {
                return false;
            }
            if (coeff_count > 2 && !is_zero_poly(poly + coeff_uint64_count, coeff_count - 2, 
                coeff_uint64_count))
            {
                return false;
            }
            return true;
        }

        inline std::size_t get_significant_coeff_count_poly(const std::uint64_t *poly, 
            std::size_t coeff_count, std::size_t coeff_uint64_count)
        {
#ifdef SEAL_DEBUG
            if (poly == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("poly");
            }
#endif
            if(coeff_count == 0)
            {
                return 0;
            }

            poly += (coeff_count - 1) * coeff_uint64_count;
            for (std::size_t i = coeff_count; i > 0; i--)
            {
                if (!is_zero_uint(poly, coeff_uint64_count))
                {
                    return i;
                }
                poly -= coeff_uint64_count;
            }
            return 0;
        }

        inline auto duplicate_poly_if_needed(const std::uint64_t *poly, 
            std::size_t coeff_count, std::size_t coeff_uint64_count, std::size_t new_coeff_count, 
            std::size_t new_coeff_uint64_count, bool force, MemoryPool &pool)
        {
#ifdef SEAL_DEBUG
            if (poly == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("poly");
            }
#endif
            if (!force && coeff_count >= new_coeff_count && 
                coeff_uint64_count == new_coeff_uint64_count)
            {
                return ConstPointer<std::uint64_t>::Aliasing(poly);
            }
            auto allocation(allocate_poly(new_coeff_count, new_coeff_uint64_count, pool));
            set_poly_poly(poly, coeff_count, coeff_uint64_count, new_coeff_count, 
                new_coeff_uint64_count, allocation.get());
            return ConstPointer<std::uint64_t>(std::move(allocation));
        }

        inline auto duplicate_bigpolyarray_if_needed(const std::uint64_t *operand, 
            std::size_t size, std::size_t coeff_count, std::size_t coeff_uint64_count, 
            std::size_t new_size, std::size_t new_coeff_count, 
            std::size_t new_coeff_uint64_count, bool force, MemoryPool &pool)
        {
#ifdef SEAL_DEBUG
            if (operand == nullptr && coeff_count > 0 && coeff_uint64_count > 0 && size > 0)
            {
                throw std::invalid_argument("operand");
            }
#endif
            if (!force && coeff_count == new_coeff_count && 
                coeff_uint64_count == new_coeff_uint64_count && size >= new_size)
            {
                return ConstPointer<std::uint64_t>::Aliasing(operand);
            }
            auto allocation(allocate_uint(new_size * new_coeff_count * new_coeff_uint64_count, pool));
            set_bigpolyarray_bigpolyarray(operand, size, coeff_count, coeff_uint64_count, 
                new_size, new_coeff_count, new_coeff_uint64_count, allocation.get());
            return ConstPointer<std::uint64_t>(std::move(allocation));
        }
        
        inline bool are_poly_coefficients_less_than(const std::uint64_t *poly, 
            std::size_t coeff_count, std::size_t coeff_uint64_count, 
            const std::uint64_t *max_coeff, std::size_t max_coeff_uint64_count)
        {
#ifdef SEAL_DEBUG
            if (poly == nullptr && coeff_count > 0 && coeff_uint64_count > 0)
            {
                throw std::invalid_argument("poly");
            }
            if (max_coeff == nullptr && max_coeff_uint64_count > 0)
            {
                throw std::invalid_argument("max_coeff");
            }
#endif
            if (coeff_count == 0)
            {
                return true;
            }
            if (max_coeff_uint64_count == 0)
            {
                return false;
            }
            if (coeff_uint64_count == 0)
            {
                return true;
            }
            for (std::size_t i = 0; i < coeff_count; i++)
            {
                if (compare_uint_uint(poly, coeff_uint64_count, max_coeff, max_coeff_uint64_count) >= 0)
                {
                    return false;
                }
                poly += coeff_uint64_count;
            }
            return true;
        }
    }
}
