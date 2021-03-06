#pragma once

// Bounds for bit-length of user-defined coefficient moduli
#define SEAL_USER_MOD_BIT_COUNT_MAX 60
#define SEAL_USER_MOD_BIT_COUNT_MIN 1

// Bounds for number of coefficient moduli
#define SEAL_COEFF_MOD_COUNT_MAX 62
#define SEAL_COEFF_MOD_COUNT_MIN 1

// Bounds for polynomial modulus degree
#define SEAL_POLY_MOD_DEGREE_MAX 65536
#define SEAL_POLY_MOD_DEGREE_MIN 2

// Bounds for the plaintext modulus
#define SEAL_PLAIN_MOD_MIN 2
#define SEAL_PLAIN_MOD_MAX (1ULL << SEAL_USER_MOD_BIT_COUNT_MAX) - 1

// Upper bound on the size of a ciphertext
#define SEAL_CIPHERTEXT_SIZE_MIN 2
#define SEAL_CIPHERTEXT_SIZE_MAX 32768

// Bounds for decomposition bit count
#define SEAL_DBC_MAX 60
#define SEAL_DBC_MIN 1

// Debugging help
#define SEAL_ASSERT(condition) { if(!(condition)){ std::cerr << "ASSERT FAILED: "   \
    << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; } }

// String expansion
#define _SEAL_STRINGIZE(x) #x
#define SEAL_STRINGIZE(x) _SEAL_STRINGIZE(x)

// Check that double is 64 bits
static_assert(sizeof(double) == 8, "Require sizeof(double) == 8");

// Check that int is 32 bits
static_assert(sizeof(int) == 4, "Require sizeof(int) == 4");

// Check that unsigned long long is 64 bits
static_assert(sizeof(unsigned long long) == 8, "Require sizeof(unsigned long long) == 8");

// Use std::byte as byte type
#if defined(SEAL_USE_STD_BYTE)
#include <cstddef>
namespace seal
{
    using SEAL_BYTE = std::byte;
}
#else
namespace seal
{
    enum class SEAL_BYTE : unsigned char {};
}
#endif

// Detect compiler
#define SEAL_COMPILER_MSVC 1
#define SEAL_COMPILER_CLANG 2
#define SEAL_COMPILER_GCC 3

#if defined(_MSC_VER)
#define SEAL_COMPILER SEAL_COMPILER_MSVC
#elif defined(__clang__)
#define SEAL_COMPILER SEAL_COMPILER_CLANG
#elif defined(__GNUC__) && !defined(__clang__)
#define SEAL_COMPILER SEAL_COMPILER_GCC
#endif

// MSVC support
#include "seal/util/msvc.h"

// clang support
#include "seal/util/clang.h"

// gcc support
#include "seal/util/gcc.h"

// Use `if constexpr' from C++17
#ifdef SEAL_USE_IF_CONSTEXPR
#define SEAL_IF_CONSTEXPR if constexpr
#else
#define SEAL_IF_CONSTEXPR if 
#endif

// Use [[maybe_unused]] from C++17
#ifdef SEAL_USE_MAYBE_UNUSED
#define SEAL_MAYBE_UNUSED [[maybe_unused]]
#else
#define SEAL_MAYBE_UNUSED  
#endif

// Which random number generator factory to use by default
#ifdef SEAL_USE_AES_NI_PRNG
// AES-PRNG with seed from std::random_device
#define SEAL_DEFAULT_RNG_FACTORY FastPRNGFactory<>
#else
// std::random_device
#define SEAL_DEFAULT_RNG_FACTORY StandardRandomAdapterFactory<std::random_device>
#endif

// Use generic functions as (slower) fallback
#ifndef SEAL_ADD_CARRY_UINT64
#define SEAL_ADD_CARRY_UINT64(operand1, operand2, carry, result) add_uint64_generic(operand1, operand2, carry, result)
#endif

#ifndef SEAL_SUB_BORROW_UINT64
#define SEAL_SUB_BORROW_UINT64(operand1, operand2, borrow, result) sub_uint64_generic(operand1, operand2, borrow, result)
#endif

#ifndef SEAL_MULTIPLY_UINT64
#define SEAL_MULTIPLY_UINT64(operand1, operand2, result128) {                      \
    multiply_uint64_generic(operand1, operand2, result128);                        \
}
#endif

#ifndef SEAL_MULTIPLY_UINT64_HW64
#define SEAL_MULTIPLY_UINT64_HW64(operand1, operand2, hw64) {                      \
    multiply_uint64_hw64_generic(operand1, operand2, hw64);                        \
}
#endif

#ifndef SEAL_MSB_INDEX_UINT64
#define SEAL_MSB_INDEX_UINT64(result, value) get_msb_index_generic(result, value)
#endif

// Template for creating functions taking any 64-bit unsigned integer
// parameter such as std::uint64_t, unsigned long, unsigned long long
// which might not be typedefs
#include <type_traits>
#include <cstdint>
namespace seal
{
    template<typename T, typename... Args>
    struct is_uint64_t : public is_uint64_t<Args...>
    {
        template<
            typename = std::enable_if_t<std::is_integral<T>::value>,
            typename = std::enable_if_t<(sizeof(T) == sizeof(std::uint64_t))>,
            typename = std::enable_if_t<std::is_unsigned<T>::value>>
        using type = T;
    };
}

// Multiplication by a plaintext zero should not be allowed, and by default SEAL 
// throws an exception in this case. For performance reasons one might want to 
// undefine this if appropriate checks are guaranteed to be performed elsewhere.
#undef SEAL_THROW_ON_MULTIPLY_PLAIN_BY_ZERO
