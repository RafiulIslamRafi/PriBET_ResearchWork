#pragma once

#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>
#include "seal/util/defines.h"
#include "seal/context.h"
#include "seal/memorymanager.h"
#include "seal/intarray.h"

namespace seal
{
    /**
    Class to store a ciphertext element. The data for a ciphertext consists 
    of two or more polynomials, which are in SEAL stored in a CRT form with 
    respect to the factors of the coefficient modulus. This data itself is 
    not meant to be modified directly by the user, but is instead operated 
    on by functions in the Evaluator class. The size of the backing array of 
    a ciphertext depends on the encryption parameters and the size of the 
    ciphertext (at least 2). If the degree of the poly_modulus encryption 
    parameter is N, and the number of primes in the coeff_modulus encryption 
    parameter is K, then the ciphertext backing array requires precisely 
    8*N*K*size bytes of memory. A ciphertext also carries with it the 
    parms_id of its associated encryption parameters, which is used to check 
    the validity of the ciphertext for homomorphic operations and decryption.

    @par Memory Management
    The size of a ciphertext refers to the number of polynomials it contains,
    whereas its capacity refers to the number of polynomials that fit in the 
    current memory allocation. In high-performance applications unnecessary 
    re-allocations should be avoided by reserving enough memory for the 
    ciphertext to begin with either by providing the desired capacity to the 
    constructor as an extra argument, or by calling the reserve function at 
    any time.

    @par Thread Safety
    In general, reading from ciphertext is thread-safe as long as no other 
    thread is concurrently mutating it. This is due to the underlying data 
    structure storing the ciphertext not being thread-safe.

    @see Plaintext for the class that stores plaintexts.
    */
    class Ciphertext
    {
        friend class Evaluator;

        friend class Encryptor;

        friend class CKKSRecryptor;

    public:
        using ct_coeff_type = std::uint64_t;

        using size_type = IntArray<ct_coeff_type>::size_type;

        /**
        Constructs an empty ciphertext allocating no memory.

        @param[in] pool The MemoryPoolHandle pointing to a valid memory pool
        @throws std::invalid_argument if pool is uninitialized
        */
        Ciphertext(const MemoryPoolHandle &pool = MemoryManager::GetPool()) :
            data_(pool)
        {
        }

        /**
        Constructs an empty ciphertext with capacity 2. In addition to the 
        capacity, the allocation size is determined by the highest-level
        parameters associated to the given SEALContext.

        @param[in] context The SEALContext
        @param[in] pool The MemoryPoolHandle pointing to a valid memory pool
        @throws std::invalid_argument if the context is not set or encryption
        parameters are not valid
        @throws std::invalid_argument if pool is uninitialized
        */
        explicit Ciphertext(const std::shared_ptr<SEALContext> &context,
            const MemoryPoolHandle &pool = MemoryManager::GetPool()) :
            data_(pool)
        {
            // Allocate memory but don't resize
            reserve(context, 2);
        }

        /**
        Constructs an empty ciphertext with capacity 2. In addition to the 
        capacity, the allocation size is determined by the encryption parameters 
        with given parms_id.

        @param[in] context The SEALContext
        @param[in] parms_id The parms_id corresponding to the encryption
        parameters to be used
        @param[in] pool The MemoryPoolHandle pointing to a valid memory pool
        @throws std::invalid_argument if the context is not set or encryption
        parameters are not valid
        @throws std::invalid_argument if parms_id is not valid for the encryption 
        parameters
        @throws std::invalid_argument if pool is uninitialized
        */
        explicit Ciphertext(const std::shared_ptr<SEALContext> &context, 
            parms_id_type parms_id,
            const MemoryPoolHandle &pool = MemoryManager::GetPool()) :
            data_(pool)
        {
            // Allocate memory but don't resize
            reserve(context, parms_id, 2);
        }

        /**
        Constructs an empty ciphertext with given capacity. In addition to 
        the capacity, the allocation size is determined by the given 
        encryption parameters.

        @param[in] context The SEALContext
        @param[in] parms_id The parms_id corresponding to the encryption
        parameters to be used
        @param[in] size_capacity The capacity
        @param[in] pool The MemoryPoolHandle pointing to a valid memory pool
        @throws std::invalid_argument if the context is not set or encryption
        parameters are not valid
        @throws std::invalid_argument if parms_id is not valid for the encryption 
        parameters
        @throws std::invalid_argument if size_capacity is less than 2 or too large
        @throws std::invalid_argument if pool is uninitialized
        */
        explicit Ciphertext(const std::shared_ptr<SEALContext> &context, 
            parms_id_type parms_id, size_type size_capacity,
            const MemoryPoolHandle &pool = MemoryManager::GetPool()) :
            data_(pool)
        {
            // Allocate memory but don't resize
            reserve(context, parms_id, size_capacity);
        }

        /**
        Constructs a new ciphertext by copying a given one.

        @param[in] copy The ciphertext to copy from
        */
        Ciphertext(const Ciphertext &copy) = default;

        /**
        Creates a new ciphertext by moving a given one.

        @param[in] source The ciphertext to move from
        */
        Ciphertext(Ciphertext &&source) = default;

        /**
        Allocates enough memory to accommodate the backing array of a ciphertext 
        with given capacity. In addition to the capacity, the allocation size is 
        determined by the encryption parameters corresponing to the given 
        parms_id.

        @param[in] context The SEALContext
        @param[in] parms_id The parms_id corresponding to the encryption
        parameters to be used
        @param[in] size_capacity The capacity
        @throws std::invalid_argument if the context is not set or encryption
        parameters are not valid
        @throws std::invalid_argument if parms_id is not valid for the encryption 
        parameters
        @throws std::invalid_argument if size_capacity is less than 2 or too large
        */
        inline void reserve(const std::shared_ptr<SEALContext> &context,
            parms_id_type parms_id, size_type size_capacity)
        {
            // Verify parameters
            if (!context)
            {
                throw std::invalid_argument("invalid context");
            }
            if (!context->parameters_set())
            {
                throw std::invalid_argument("encryption parameters are not set correctly");
            }

            auto context_data_ptr = context->context_data(parms_id);
            if (!context_data_ptr)
            {
                throw std::invalid_argument("parms_id is not valid for encryption parameters");
            }

            // Need to set parms_id first
            auto &parms = context_data_ptr->parms();
            parms_id_ = parms.parms_id();

            reserve_internal(size_capacity, parms.poly_modulus_degree(),
                static_cast<size_type>(parms.coeff_modulus().size()));
        }

        /**
        Allocates enough memory to accommodate the backing array of a ciphertext
        with given capacity. In addition to the capacity, the allocation size is 
        determined by the highest-level parameters associated to the given 
        SEALContext.

        @param[in] context The SEALContext
        @param[in] size_capacity The capacity
        @throws std::invalid_argument if the context is not set or encryption
        parameters are not valid
        @throws std::invalid_argument if size_capacity is less than 2 or too large
        */
        inline void reserve(const std::shared_ptr<SEALContext> &context,
            size_type size_capacity)
        {
            // Verify parameters
            if (!context)
            {
                throw std::invalid_argument("invalid context");
            }
            reserve(context, context->first_parms_id(), size_capacity);
        }

        /**
        Allocates enough memory to accommodate the backing array of a ciphertext 
        with given capacity. In addition to the capacity, the allocation size is 
        determined by the current encryption parameters.

        @param[in] size_capacity The capacity
        @throws std::invalid_argument if size_capacity is less than 2 or too large
        @throws std::logic_error if the encryption parameters are not  
        */
        inline void reserve(size_type size_capacity)
        {
            // Note: poly_modulus_degree_ and coeff_mod_count_ are either valid 
            // or coeff_mod_count_ is zero (in which case no memory is allocated).
            reserve_internal(size_capacity, poly_modulus_degree_, 
                coeff_mod_count_);
        }

        /**
        Resizes the ciphertext to given size, reallocating if the capacity 
        of the ciphertext is too small. The ciphertext parameters are 
        determined by the given SEALContext and parms_id.
        
        This function is mainly intended for internal use and is called
        automatically by functions such as Evaluator::multiply and
        Evaluator::relinearize. A normal user should never have a reason
        to manually resize a ciphertext.

        @param[in] context The SEALContext
        @param[in] parms_id The parms_id corresponding to the encryption
        parameters to be used
        @param[in] size The new size
        @throws std::invalid_argument if the context is not set or encryption
        parameters are not valid
        @throws std::invalid_argument if parms_id is not valid for the encryption
        parameters
        @throws std::invalid_argument if size is less than 2 or too large
        */
        inline void resize(const std::shared_ptr<SEALContext> &context,
            parms_id_type parms_id, size_type size)
        {
            // Verify parameters
            if (!context)
            {
                throw std::invalid_argument("invalid context");
            }
            if (!context->parameters_set())
            {
                throw std::invalid_argument("encryption parameters are not set correctly");
            }

            auto context_data_ptr = context->context_data(parms_id);
            if (!context_data_ptr)
            {
                throw std::invalid_argument("parms_id is not valid for encryption parameters");
            }

            // Need to set parms_id first
            auto &parms = context_data_ptr->parms();
            parms_id_ = parms.parms_id();

            resize_internal(size, parms.poly_modulus_degree(),
                static_cast<size_type>(parms.coeff_modulus().size()));
        }

        /**
        Resizes the ciphertext to given size, reallocating if the capacity
        of the ciphertext is too small. The ciphertext parameters are
        determined by the highest-level parameters associated to the given 
        SEALContext. 
        
        This function is mainly intended for internal use and is called 
        automatically by functions such as Evaluator::multiply and 
        Evaluator::relinearize. A normal user should never have a reason 
        to manually resize a ciphertext.

        @param[in] context The SEALContext
        @param[in] size The new size
        @throws std::invalid_argument if the context is not set or encryption
        parameters are not valid
        @throws std::invalid_argument if size is less than 2 or too large
        */
        inline void resize(const std::shared_ptr<SEALContext> &context,
            size_type size)
        {
            // Verify parameters
            if (!context)
            {
                throw std::invalid_argument("invalid context");
            }
            resize(context, context->first_parms_id(), size);
        }

        /**
        Resizes the ciphertext to given size, reallocating if the capacity
        of the ciphertext is too small.

        This function is mainly intended for internal use and is called
        automatically by functions such as Evaluator::multiply and
        Evaluator::relinearize. A normal user should never have a reason
        to manually resize a ciphertext.

        @param[in] size The new size
        @throws std::invalid_argument if size is less than 2 or too large
        */
        inline void resize(size_type size)
        {
            // Note: poly_modulus_degree_ and coeff_mod_count_ are either valid 
            // or coeff_mod_count_ is zero (in which case no memory is allocated).
            resize_internal(size, poly_modulus_degree_, coeff_mod_count_);
        }

        /**
        Resets the ciphertext. This function releases any memory allocated 
        by the ciphertext, returning it to the memory pool. It also sets all 
        encryption parameter specific size information to zero.
        */
        inline void release() noexcept
        {
            parms_id_ = parms_id_zero;
            is_ntt_form_ = false;
            size_capacity_ = 2;
            size_ = 0;
            poly_modulus_degree_ = 0;
            coeff_mod_count_ = 0;
            scale_ = 1.0;
            data_.release();
        }

        /**
        Copies a given ciphertext to the current one.

        @param[in] assign The ciphertext to copy from
        */
        Ciphertext &operator =(const Ciphertext &assign);

        /**
        Moves a given ciphertext to the current one.

        @param[in] assign The ciphertext to move from
        */
        Ciphertext &operator =(Ciphertext &&assign) = default;

        /**
        Returns a pointer to the beginning of the ciphertext data.
        */
        inline ct_coeff_type *data() noexcept
        {
            return data_.begin();
        }

        /**
        Returns a const pointer to the beginning of the ciphertext data.
        */
        inline const ct_coeff_type *data() const noexcept
        {
            return data_.cbegin();
        }
#ifdef SEAL_USE_MSGSL_MULTISPAN
        /**
        Returns the ciphertext data.
        */
        inline gsl::multi_span<
            ct_coeff_type,
            gsl::dynamic_range,
            gsl::dynamic_range,
            gsl::dynamic_range> data_span()
        {
            return gsl::as_multi_span<
                ct_coeff_type,
                gsl::dynamic_range,
                gsl::dynamic_range,
                gsl::dynamic_range>(
                    data_.begin(),
                    static_cast<std::ptrdiff_t>(size_),
                    static_cast<std::ptrdiff_t>(coeff_mod_count_),
                    static_cast<std::ptrdiff_t>(poly_modulus_degree_));
        }

        /**
        Returns the backing array storing all of the coefficient values.
        */
        inline gsl::multi_span<
            const ct_coeff_type,
            gsl::dynamic_range,
            gsl::dynamic_range,
            gsl::dynamic_range> data_span() const
        {
            return gsl::as_multi_span<
                const ct_coeff_type,
                gsl::dynamic_range,
                gsl::dynamic_range,
                gsl::dynamic_range>(
                    data_.cbegin(),
                    static_cast<std::ptrdiff_t>(size_),
                    static_cast<std::ptrdiff_t>(coeff_mod_count_),
                    static_cast<std::ptrdiff_t>(poly_modulus_degree_));
        }
#endif
        /**
        Returns a pointer to a particular polynomial in the ciphertext 
        data. Note that SEAL stores each polynomial in the ciphertext 
        modulo all of the K primes in the coefficient modulus. The pointer 
        returned by this function is to the beginning (constant coefficient)
        of the first one of these K polynomials.

        @param[in] poly_index The index of the polynomial in the ciphertext
        @throws std::out_of_range if poly_index is less than 0 or bigger 
        than the size of the ciphertext
        */
        inline ct_coeff_type *data(size_type poly_index)
        {
            auto poly_uint64_count = poly_modulus_degree_ * coeff_mod_count_;
            if (poly_uint64_count == 0)
            {
                return nullptr;
            }
            if (poly_index >= size_)
            {
                throw std::out_of_range("poly_index must be within [0, size)");
            }
            return data_.begin() + poly_index * poly_uint64_count;
        }

        /**
        Returns a const pointer to a particular polynomial in the 
        ciphertext data. Note that SEAL stores each polynomial in the 
        ciphertext modulo all of the K primes in the coefficient modulus. 
        The pointer returned by this function is to the beginning 
        (constant coefficient) of the first one of these K polynomials.

        @param[in] poly_index The index of the polynomial in the ciphertext
        @throws std::out_of_range if poly_index is out of range
        */
        inline const ct_coeff_type *data(size_type poly_index) const
        {
            auto poly_uint64_count = poly_modulus_degree_ * coeff_mod_count_;
            if (poly_uint64_count == 0)
            {
                return nullptr;
            }
            if (poly_index >= size_)
            {
                throw std::out_of_range("poly_index must be within [0, size)");
            }
            return data_.cbegin() + poly_index * poly_uint64_count;
        }

        /**
        Returns a reference to a polynomial coefficient at a particular 
        index in the ciphertext data. If the polynomial modulus has degree N, 
        and the number of primes in the coefficient modulus is K, then the 
        ciphertext contains size*N*K coefficients. Thus, the coeff_index has 
        a range of [0, size*N*K).

        @param[in] coeff_index The index of the coefficient
        @throws std::out_of_range if coeff_index is out of range
        */
        inline ct_coeff_type &operator [](size_type coeff_index)
        {
            return data_.at(coeff_index);
        }

        /**
        Returns a const reference to a polynomial coefficient at a particular 
        index in the ciphertext data. If the polynomial modulus has degree N, 
        and the number of primes in the coefficient modulus is K, then the 
        ciphertext contains size*N*K coefficients. Thus, the coeff_index has 
        a range of [0, size*N*K).

        @param[in] coeff_index The index of the coefficient
        @throws std::out_of_range if coeff_index is out of range 
        */
        inline const ct_coeff_type &operator [](size_type coeff_index) const
        {
            return data_.at(coeff_index);
        }

        /**
        Returns the number of primes in the coefficient modulus of the 
        associated encryption parameters. This directly affects the 
        allocation size of the ciphertext.
        */
        inline size_type coeff_mod_count() const noexcept
        {
            return coeff_mod_count_;
        }

        /**
        Returns the degree of the polynomial modulus of the associated 
        encryption parameters. This directly affects the allocation size 
        of the ciphertext.
        */
        inline size_type poly_modulus_degree() const noexcept
        {
            return poly_modulus_degree_;
        }

        /**
        Returns the capacity of the allocation. This means the largest size 
        of the ciphertext that can be stored in the current allocation with 
        the current encryption parameters.
        */
        inline size_type size_capacity() const noexcept
        {
            return size_capacity_;
        }

        /**
        Returns the size of the ciphertext.
        */
        inline size_type size() const noexcept
        {
            return size_;
        }

        /**
        Returns the total size of the current allocation in 64-bit words.
        */
        inline size_type uint64_count_capacity() const noexcept
        {
            return data_.capacity();
        }

        /**
        Returns the total size of the current ciphertext in 64-bit words.
        */
        inline size_type uint64_count() const noexcept
        {
            return data_.size();
        }

        /**
        Saves the ciphertext to an output stream. The output is in binary 
        format and not human-readable. The output stream must have the 
        "binary" flag set.

        @param[in] stream The stream to save the ciphertext to
        @see load() to load a saved ciphertext.
        */
        void save(std::ostream &stream) const;

        /**
        Loads a ciphertext from an input stream overwriting the current 
        ciphertext.

        @param[in] stream The stream to load the ciphertext from
        @throws std::invalid_argument if the loaded ciphertext data is invalid
        @see save() to save a ciphertext.
        */
        void load(std::istream &stream);

        /**
        Returns whether the ciphertext is in NTT form.
        */
        inline bool is_ntt_form() const noexcept
        {
            return is_ntt_form_;
        }

        /**
        Returns a reference to parms_id.

        @see EncryptionParameters for more information about parms_id.
        */
        inline auto &parms_id() noexcept
        {
            return parms_id_;
        }

        /**
        Returns a const reference to parms_id.

        @see EncryptionParameters for more information about parms_id.
        */
        inline auto &parms_id() const noexcept
        {
            return parms_id_;
        }

        /**
        Returns a reference to the scale. This is only needed when using the
        CKKS encryption scheme. The user should have little or no reason to ever
        change the scale by hand.
        */
        inline auto &scale() noexcept
        {
            return scale_;
        }

        /**
        Returns a constant reference to the scale. This is only needed when
        using the CKKS encryption scheme.
        */
        inline auto &scale() const noexcept
        {
            return scale_;
        }

        /**
        Returns the currently used MemoryPoolHandle.
        */
        inline MemoryPoolHandle pool() const noexcept
        {
            return data_.pool();
        }

    private:
        void reserve_internal(size_type size_capacity, size_type poly_modulus_degree,
            size_type coeff_mod_count);

        void resize_internal(size_type size, size_type poly_modulus_degree,
            size_type coeff_mod_count);

        parms_id_type parms_id_ = parms_id_zero;

        bool is_ntt_form_ = false;
        
        size_type size_capacity_ = 2;

        size_type size_ = 0;

        size_type poly_modulus_degree_ = 0;

        size_type coeff_mod_count_ = 0;

        double scale_ = 1.0;

        IntArray<ct_coeff_type> data_;
    };
}
