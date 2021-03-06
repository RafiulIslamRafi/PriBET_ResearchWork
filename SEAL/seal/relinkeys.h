#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include "seal/ciphertext.h"
#include "seal/memorymanager.h"
#include "seal/encryptionparams.h"

namespace seal
{
    /**
    Class to store relinearization keys. An relinearization key has type std::vector<Ciphertext>.
    An instance of the RelinKeys class stores internally an std::vector of relinearization
    keys.
    
    @par Relinearization
    Concretely, an relinearization key corresponding to a power K of the secret key can be used
    in the relinearization operation to change a ciphertext of size K+1 to size K. Recall
    that the smallest possible size for a ciphertext is 2, so the first relinearization key is
    corresponds to the square of the secret key. The second relinearization key corresponds to
    the cube of the secret key, and so on. For example, to relinearize a ciphertext of size
    7 back to size 2, one would need 5 relinearization keys, although it is hard to imagine 
    a situation where it makes sense to have size 7 ciphertexts, as operating on such objects
    would be very slow. Most commonly only one relinearization key is needed, and relinearization
    is performed after every multiplication.

    @par Decomposition Bit Count
    Decomposition bit count (dbc) is a parameter that describes a performance trade-off in
    the relinearization process. Namely, in the relinearization process the polynomials in 
    the ciphertexts (with large coefficients) get decomposed into a smaller base 2^dbc,
    coefficient-wise. Each of the decomposition factors corresponds to a piece of data in
    the relinearization key, so the smaller the dbc is, the larger the relinearization keys are.
    Moreover, a smaller dbc results in less invariant noise budget being consumed in the
    relinearization process. However, using a large dbc is much faster, and often one 
    would want to optimize the dbc to be as large as possible for performance. The dbc is 
    upper-bounded by the value of 60, and lower-bounded by the value of 1.

    @par Thread Safety
    In general, reading from RelinKeys is thread-safe as long as no other thread is
    concurrently mutating it. This is due to the underlying data structure storing the 
    relinearization keys not being thread-safe.

    @see SecretKey for the class that stores the secret key.
    @see PublicKey for the class that stores the public key.
    @see GaloisKeys for the class that stores the Galois keys.
    @see KeyGenerator for the class that generates the relinearization keys.
    */
    class RelinKeys
    {
        friend class KeyGenerator;

    public:
        /**
        Creates an empty set of relinearization keys.
        */
        RelinKeys() = default;

        /**
        Creates a new RelinKeys instance by copying a given instance.

        @param[in] copy The RelinKeys to copy from
        */
        RelinKeys(const RelinKeys &copy) = default;

        /**
        Creates a new RelinKeys instance by moving a given instance.

        @param[in] source The RelinKeys to move from
        */
        RelinKeys(RelinKeys &&source) = default;

        /**
        Copies a given RelinKeys instance to the current one.

        @param[in] assign The RelinKeys to copy from
        */
        RelinKeys &operator =(const RelinKeys &assign);

        /**
        Moves a given RelinKeys instance to the current one.

        @param[in] assign The RelinKeys to move from
        */
        RelinKeys &operator =(RelinKeys &&assign) = default;

        /**
        Returns the current number of relinearization keys.
        */
        inline std::size_t size() const
        {
            return keys_.size();
        }

        /**
        Returns the decomposition bit count.
        */
        inline int decomposition_bit_count() const noexcept
        {
            return decomposition_bit_count_;
        }

        /**
        Returns a reference to the relinearization keys data.
        */
        inline auto &data() noexcept
        {
            return keys_;
        }

        /**
        Returns a const reference to the relinearization keys data.
        */
        inline auto &data() const noexcept
        {
            return keys_;
        }

        /**
        Returns a const reference to an relinearization key. The returned relinearization key 
        corresponds to the given power of the secret key.

        @param[in] key_power The power of the secret key
        @throw std::invalid_argument if the key corresponding to key_power does not exist
        */
        inline auto &key(int key_power) const
        {
            if (!has_key(key_power))
            {
                throw std::invalid_argument("requested key does not exist");
            }
            return keys_[static_cast<std::size_t>(key_power) - 2];
        }

        /**
        Returns whether an relinearization key corresponding to a given power of the secret key 
        exists.

        @param[in] key_power The power of the secret key
        */
        inline bool has_key(int key_power) const noexcept
        {
            return key_power >= 2 && 
                keys_.size() >= static_cast<std::size_t>(key_power - 1);
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
        Saves the RelinKeys instance to an output stream. The output is in binary format 
        and not human-readable. The output stream must have the "binary" flag set.

        @param[in] stream The stream to save the RelinKeys to
        @see load() to load a saved RelinKeys instance.
        */
        void save(std::ostream &stream) const;

        /**
        Loads an RelinKeys instance from an input stream overwriting the current
        RelinKeys instance.

        @param[in] stream The stream to load the RelinKeys instance from
        @see save() to save an RelinKeys instance.
        */
        void load(std::istream &stream);

        /**
        Returns the currently used MemoryPoolHandle.
        */
        inline MemoryPoolHandle pool() const noexcept
        {
            return pool_;
        }

    private:
        MemoryPoolHandle pool_ = MemoryManager::GetPool();

        parms_id_type parms_id_ = parms_id_zero;

        /**
        The vector of relinearization keys.
        */
        std::vector<std::vector<Ciphertext>> keys_{};

        int decomposition_bit_count_ = 0;
    };
}
