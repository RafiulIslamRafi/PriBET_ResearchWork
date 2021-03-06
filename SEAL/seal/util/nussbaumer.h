#pragma once

#include <cstdint>
#include "seal/util/pointer.h"

namespace seal
{
    namespace util
    {
        void nussbaumer_multiply_poly_poly(const std::uint64_t *operand1, const std::uint64_t *operand2, 
            int coeff_count_power, std::size_t coeff_uint64_count, std::size_t sum_uint64_count, 
            std::size_t product_uint64_count, std::uint64_t *result, MemoryPool &pool);
        
        void nussbaumer_cross_multiply_poly_poly(const std::uint64_t *operand1, 
            const std::uint64_t *operand2, int coeff_count_power, std::size_t coeff_uint64_count, 
            std::size_t sum_uint64_count, std::size_t product_uint64_count, std::uint64_t *result_op1_op1, 
            std::uint64_t *result_op2_op2, std::uint64_t *result_op1_op2, MemoryPool &pool);
    }
}