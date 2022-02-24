#pragma once

#if SEAL_COMPILER == SEAL_COMPILER_MSVC

// Current version
#define SEAL_VERSION "3.0.0"

// Require Visual Studio 2017 version 15.3 or newer
#if (_MSC_VER < 1911)
#error "Microsoft Visual Studio 2017 version 15.3 or newer required"
#endif

// Deal with Debug mode in Visual Studio
#ifdef _DEBUG
#define SEAL_DEBUG
#endif

// Try to check presence of additional headers using __has_include
#ifdef __has_include

// Check for AES-NI
#if __has_include(<wmmintrin.h>)
#define SEAL_USE_AES_NI_PRNG
#else
#undef SEAL_USE_AES_NI_PRNG
#endif //__has_include(<wmmintrin.h>)

// Check for MSGSL
#if __has_include(<gsl/gsl>)
#include <gsl/gsl>
#define SEAL_USE_MSGSL
#else
#undef SEAL_USE_MSGSL
#endif //__has_include(<gsl/gsl>)

#endif

// Are we compiling with C++17 or newer
#if (__cplusplus >= 201703L)
// Use `if constexpr'
#define SEAL_USE_IF_CONSTEXPR

// Use [[maybe_unused]]
#define SEAL_USE_MAYBE_UNUSED
#else
#undef SEAL_USE_IF_CONSTEXPR
#undef SEAL_USE_MAYBE_UNUSED
#endif

// X64
#ifdef _M_X64

// Use compiler intrinsics for better performance
#define SEAL_USE_INTRIN

#ifdef SEAL_USE_INTRIN
#include <intrin.h>

#pragma intrinsic(_addcarry_u64)
#define SEAL_ADD_CARRY_UINT64(operand1, operand2, carry, result) _addcarry_u64(     \
    carry, operand1, operand2, result)

#pragma intrinsic(_subborrow_u64)
#define SEAL_SUB_BORROW_UINT64(operand1, operand2, borrow, result) _subborrow_u64(  \
    borrow, operand1, operand2, result)

#pragma intrinsic(_BitScanReverse64)
#define SEAL_MSB_INDEX_UINT64(result, value) _BitScanReverse64(result, value)

#pragma intrinsic(_umul128)
#define SEAL_MULTIPLY_UINT64(operand1, operand2, result128) {                       \
    result128[0] = _umul128(operand1, operand2, result128 + 1);                     \
}
#define SEAL_MULTIPLY_UINT64_HW64(operand1, operand2, hw64) {                       \
    _umul128(operand1, operand2, hw64);                                             \
}

#endif
#else 
#undef SEAL_USE_INTRIN

#endif //_M_X64

#endif
