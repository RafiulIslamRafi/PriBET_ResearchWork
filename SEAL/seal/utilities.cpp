#include <cmath>
#include "seal/utilities.h"
#include "seal/util/uintarith.h"
#include "seal/util/uintarithmod.h"
#include "seal/util/polyarithmod.h"
#include "seal/util/polycore.h"
#include "seal/util/polyarith.h"
#include "seal/util/modulus.h"
#include "seal/util/polymodulus.h"
#include "seal/decryptor.h"

using namespace std;
using namespace seal::util;

namespace seal
{
    namespace
    {
        ConstPointer<std::uint64_t> duplicate_uint_if_needed(const BigUInt &input, 
            size_t new_uint64_count, bool force, MemoryPool &pool)
        {
            return duplicate_uint_if_needed(input.data(), input.uint64_count(), 
                new_uint64_count, force, pool);
        }

        ConstPointer<std::uint64_t> duplicate_poly_if_needed(const BigPoly &poly, 
            size_t new_coeff_count, size_t new_coeff_uint64_count, bool force, MemoryPool &pool)
        {
            return duplicate_poly_if_needed(poly.data(), poly.coeff_count(), poly.coeff_uint64_count(), 
                new_coeff_count, new_coeff_uint64_count, force, pool);
        }
    }

    BigUInt poly_infty_norm(const BigPoly &poly)
    {
        if (poly.is_zero())
        {
            return BigUInt();
        }

        size_t coeff_count = poly.coeff_count();
        int coeff_bit_count = poly.coeff_bit_count();
        size_t coeff_uint64_count = static_cast<size_t>(
            divide_round_up(coeff_bit_count, bits_per_uint64));

        BigUInt result(coeff_bit_count);
        util::poly_infty_norm(poly.data(), coeff_count, coeff_uint64_count, result.data());

        return result;
    }

    BigUInt poly_infty_norm_coeffmod(const BigPoly &poly, const BigUInt &modulus, 
        const MemoryPoolHandle &pool)
    {
        if (modulus.is_zero())
        {
            throw invalid_argument("modulus cannot be zero");
        }
        if (!pool)
        {
            throw invalid_argument("pool is uninitialized");
        }

        if (poly.is_zero())
        {
            return BigUInt();
        }

        size_t poly_coeff_count = poly.coeff_count();
        int poly_coeff_bit_count = poly.coeff_bit_count();
        size_t poly_coeff_uint64_count = static_cast<size_t>(
            divide_round_up(poly_coeff_bit_count, bits_per_uint64));

        Modulus mod(modulus.data(), modulus.uint64_count(), pool);
        BigUInt result(modulus.significant_bit_count());
        util::poly_infty_norm_coeffmod(poly.data(), poly_coeff_count, poly_coeff_uint64_count, 
            mod, result.data(), pool);

        return result;
    }

    void exponentiate_uint_mod(const BigUInt &operand, const BigUInt &exponent, 
        const BigUInt &modulus, BigUInt &destination, const MemoryPoolHandle &pool)
    {
        if (operand.significant_bit_count() > modulus.significant_bit_count())
        {
            throw invalid_argument("operand is not reduced");
        }
        if (operand.is_zero() && exponent == 0)
        {
            throw invalid_argument("undefined operation");
        }
        if (!pool)
        {
            throw invalid_argument("pool is uninitialized");
        }

        if (operand.is_zero())
        {
            destination.set_zero();
            return;
        }

        if (destination.bit_count() != modulus.significant_bit_count())
        {
            destination.resize(modulus.significant_bit_count());
        }

        auto operand_ptr = duplicate_uint_if_needed(operand, modulus.uint64_count(), false, pool);
        util::exponentiate_uint_mod(operand_ptr.get(), exponent.data(), exponent.uint64_count(), 
            Modulus(modulus.data(), modulus.uint64_count(), pool), destination.data(), pool);
    }

    void exponentiate_poly_polymod_coeffmod(const BigPoly &operand, const BigUInt &exponent, 
        const BigPoly &poly_modulus, const BigUInt &coeff_modulus, BigPoly &destination, 
        const MemoryPoolHandle &pool)
    {
        if (operand.significant_coeff_count() > poly_modulus.coeff_count() ||
            operand.significant_coeff_bit_count() > coeff_modulus.significant_bit_count())
        {
            throw invalid_argument("operand is not reduced");
        }
        if (exponent < 0)
        {
            throw invalid_argument("exponent must be a non-negative integer");
        }
        if (operand.is_zero() && exponent == 0)
        {
            throw invalid_argument("undefined operation");
        }
        if (!pool)
        {
            throw invalid_argument("pool is uninitialized");
        }

        if (operand.is_zero())
        {
            destination.set_zero();
            return;
        }

        if (destination.coeff_bit_count() != coeff_modulus.significant_bit_count() || 
            destination.coeff_count() != poly_modulus.coeff_count())
        {
            destination.resize(poly_modulus.coeff_count(), coeff_modulus.significant_bit_count());
        }

        auto operand_ptr = duplicate_poly_if_needed(operand, poly_modulus.coeff_count(), 
            coeff_modulus.uint64_count(), false, pool);
        util::exponentiate_poly_polymod_coeffmod(operand_ptr.get(), exponent.data(), exponent.uint64_count(),
            PolyModulus(poly_modulus.data(), poly_modulus.coeff_count(), poly_modulus.coeff_uint64_count()), 
            Modulus(coeff_modulus.data(), coeff_modulus.uint64_count(), pool), 
            destination.data(), pool);
    }

    void poly_eval_poly(const BigPoly &poly_to_evaluate, const BigPoly &poly_to_evaluate_at, 
        BigPoly &destination, const MemoryPoolHandle &pool)
    {
        if (!pool)
        {
            throw invalid_argument("pool is uninitialized");
        }

        size_t poly_to_eval_coeff_uint64_count = static_cast<size_t>(divide_round_up(
                poly_to_evaluate.coeff_bit_count(), bits_per_uint64));
        size_t value_coeff_uint64_count = static_cast<size_t>(divide_round_up(
                poly_to_evaluate_at.coeff_bit_count(), bits_per_uint64));

        if (poly_to_evaluate.is_zero())
        {
            destination.set_zero();
            return;
        }
        if (poly_to_evaluate_at.is_zero())
        {
            destination.resize(1, poly_to_evaluate.coeff_bit_count());
            set_uint_uint(poly_to_evaluate.data(), poly_to_eval_coeff_uint64_count, destination.data());
            return;
        }

        size_t result_coeff_count = (poly_to_evaluate.significant_coeff_count() - 1) * 
            (poly_to_evaluate_at.significant_coeff_count() - 1) + 1;
        int result_coeff_bit_count = poly_to_evaluate.coeff_bit_count() + 
            static_cast<int>(poly_to_evaluate.coeff_count() - 1) * poly_to_evaluate_at.coeff_bit_count();
        size_t result_coeff_uint64_count = static_cast<size_t>(
            divide_round_up(result_coeff_bit_count, bits_per_uint64));
        destination.resize(result_coeff_count, result_coeff_bit_count);

        util::poly_eval_poly(poly_to_evaluate.data(), poly_to_evaluate.coeff_count(), 
            poly_to_eval_coeff_uint64_count, poly_to_evaluate_at.data(), 
            poly_to_evaluate_at.coeff_count(), value_coeff_uint64_count, 
            result_coeff_count, result_coeff_uint64_count, destination.data(), pool);
    }

    BigPoly poly_eval_poly(const BigPoly &poly_to_evaluate, const BigPoly &poly_to_evaluate_at, 
        const MemoryPoolHandle &pool)
    {
        BigPoly result;
        poly_eval_poly(poly_to_evaluate, poly_to_evaluate_at, result, pool);
        return result;
    }

    void poly_eval_poly_polymod_coeffmod(const BigPoly &poly_to_evaluate, 
        const BigPoly &poly_to_evaluate_at, const BigPoly &poly_modulus, 
        const BigUInt &coeff_modulus, BigPoly &destination, const MemoryPoolHandle &pool)
    {
        if (!pool)
        {
            throw invalid_argument("pool is uninitialized");
        }
        if (poly_to_evaluate.significant_coeff_count() > poly_modulus.coeff_count() ||
            poly_to_evaluate.significant_coeff_bit_count() > coeff_modulus.significant_bit_count())
        {
            throw invalid_argument("poly_to_evaluate is not reduced");
        }
        if (poly_to_evaluate_at.significant_coeff_count() > poly_modulus.coeff_count() ||
            poly_to_evaluate_at.significant_coeff_bit_count() > coeff_modulus.significant_bit_count())
        {
            throw invalid_argument("poly_to_evaluate_at is not reduced");
        }

        size_t poly_to_eval_coeff_uint64_count = poly_to_evaluate.coeff_uint64_count();
        int coeff_modulus_bit_count = coeff_modulus.significant_bit_count();

        if (poly_to_evaluate.is_zero())
        {
            destination.set_zero();
        }

        if (poly_to_evaluate_at.is_zero())
        {
            destination.resize(1, coeff_modulus_bit_count);
            modulo_uint(poly_to_evaluate.data(), poly_to_eval_coeff_uint64_count, 
                Modulus(coeff_modulus.data(), coeff_modulus.uint64_count(), pool), 
                destination.data(), pool);
            return;
        }

        auto poly_to_eval_ptr = duplicate_poly_if_needed(poly_to_evaluate, 
            poly_modulus.coeff_count(), coeff_modulus.uint64_count(), false, pool);
        auto poly_to_eval_at_ptr = duplicate_poly_if_needed(poly_to_evaluate_at, 
            poly_modulus.coeff_count(), coeff_modulus.uint64_count(), false, pool);

        destination.resize(poly_modulus.coeff_count(), coeff_modulus_bit_count);

        util::poly_eval_poly_polymod_coeffmod(poly_to_eval_ptr.get(), poly_to_eval_at_ptr.get(),
            PolyModulus(poly_modulus.data(), poly_modulus.coeff_count(), poly_modulus.coeff_uint64_count()), 
            Modulus(coeff_modulus.data(), coeff_modulus.uint64_count(), pool),
            destination.data(), pool);
    }

    BigPoly poly_eval_poly_polymod_coeffmod(const BigPoly &poly_to_evaluate, 
        const BigPoly &poly_to_evaluate_at, const BigPoly &poly_modulus, 
        const BigUInt &coeff_modulus, const MemoryPoolHandle &pool)
    {
        BigPoly result;
        poly_eval_poly_polymod_coeffmod(poly_to_evaluate, poly_to_evaluate_at, poly_modulus, 
            coeff_modulus, result, pool);
        return result;
    }
    
    void poly_eval_uint_mod(const BigPoly &poly_to_evaluate, const BigUInt &value, const BigUInt &modulus, 
        BigUInt &destination, const MemoryPoolHandle &pool)
    {
        if (poly_to_evaluate.significant_coeff_bit_count() > modulus.significant_bit_count())
        {
            throw invalid_argument("poly_to_evaluate is not reduced");
        }
        if (value.significant_bit_count() > modulus.significant_bit_count())
        {
            throw invalid_argument("value is not reduced");
        }
        if (!pool)
        {
            throw invalid_argument("pool is uninitialized");
        }

        size_t poly_to_eval_coeff_uint64_count = poly_to_evaluate.coeff_uint64_count();
        int modulus_bit_count = modulus.significant_bit_count();

        if (poly_to_evaluate.is_zero())
        {
            destination.set_zero();
        }

        if (value.is_zero())
        {
            destination.resize(modulus_bit_count);
            modulo_uint(poly_to_evaluate.data(), poly_to_eval_coeff_uint64_count,
                Modulus(modulus.data(), modulus.uint64_count(), pool), 
                destination.data(), pool);
            return;
        }

        auto value_ptr = duplicate_uint_if_needed(value, modulus.uint64_count(), false, pool);

        destination.resize(modulus_bit_count);

        util::poly_eval_uint_mod(poly_to_evaluate.data(), poly_to_evaluate.coeff_count(), value_ptr.get(), 
            Modulus(modulus.data(), modulus.uint64_count(), pool), destination.data(), pool);
    }

    BigUInt poly_eval_uint_mod(const BigPoly &poly_to_evaluate, const BigUInt &value, 
        const BigUInt &modulus, const MemoryPoolHandle &pool)
    {
        BigUInt result;
        poly_eval_uint_mod(poly_to_evaluate, value, modulus, result, pool);
        return result;
    }

    BigUInt exponentiate_uint_mod(const BigUInt &operand, const BigUInt &exponent, 
        const BigUInt &modulus, const MemoryPoolHandle &pool)
    {
        BigUInt result(modulus.significant_bit_count());
        exponentiate_uint_mod(operand, exponent, modulus, result, pool);
        return result;
    }

    BigPoly exponentiate_poly_polymod_coeffmod(const BigPoly &operand, const BigUInt &exponent,
        const BigPoly &poly_modulus, const BigUInt &coeff_modulus, const MemoryPoolHandle &pool)
    {
        BigPoly result(poly_modulus.coeff_count(), coeff_modulus.significant_bit_count());
        exponentiate_poly_polymod_coeffmod(operand, exponent, poly_modulus, coeff_modulus, result, pool);
        return result;
    }
}