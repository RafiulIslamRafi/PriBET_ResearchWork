#pragma once

#include <cstdint>
#include "seal/util/modulus.h"
#include "seal/util/pointer.h"
#include "seal/util/ntt.h"

namespace seal
{
    namespace util
    {
        void ntt_multiply_poly_poly(const std::uint64_t *operand1, 
            const std::uint64_t *operand2, const NTTTables &tables, 
            std::uint64_t *result, MemoryPool &pool);

        void ntt_multiply_poly_nttpoly(const std::uint64_t *operand1, 
            const std::uint64_t *operand2, const NTTTables &tables, 
            std::uint64_t *result, MemoryPool &pool);

        void ntt_double_multiply_poly_nttpoly(const std::uint64_t *operand1, 
            const std::uint64_t *operand2, const std::uint64_t *operand3, 
            const NTTTables &tables, std::uint64_t *result1, std::uint64_t *result2, 
            MemoryPool &pool);
        
        void nussbaumer_multiply_poly_poly_coeffmod(const std::uint64_t *operand1, 
            const std::uint64_t *operand2, int coeff_count_power, 
            const Modulus &modulus, std::uint64_t *result, MemoryPool &pool);
   
        void ntt_dot_product_bigpolyarray_nttbigpolyarray(const std::uint64_t *array1, 
            const std::uint64_t *array2, std::size_t count, std::size_t array_poly_uint64_count, 
            const NTTTables &tables, std::uint64_t *result, MemoryPool &pool);

        void ntt_double_dot_product_bigpolyarray_nttbigpolyarrays(
            const std::uint64_t *array1, const std::uint64_t *array2, 
            const std::uint64_t *array3, std::size_t count, std::size_t array_poly_uint64_count, 
            const NTTTables &tables, std::uint64_t *result1, std::uint64_t *result2, 
            MemoryPool &pool);
    
        void nussbaumer_dot_product_bigpolyarray_coeffmod(const std::uint64_t *array1, 
            const std::uint64_t *array2, std::size_t count, const PolyModulus &poly_modulus, 
            const Modulus &modulus, std::uint64_t *result, MemoryPool &pool);
    }
}