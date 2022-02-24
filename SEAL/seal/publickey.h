#pragma once

#include "seal/ciphertext.h"
#include <iostream>

namespace seal
{
    /**
    Class to store a public key.

    @par Thread Safety
    In general, reading from PublicKey is thread-safe as long as no other thread
    is concurrently mutating it. This is due to the underlying data structure 
    storing the public key not being thread-safe.

    @see KeyGenerator for the class that generates the public key.
    @see SecretKey for the class that stores the secret key.
    @see RelinKeys for the class that stores the relinearization keys.
    @see GaloisKeys for the class that stores the Galois keys.
    */
    class PublicKey
    {
        friend class KeyGenerator;

    public:
        /**
        Creates an empty public key.
        */
        PublicKey() = default;

        /**
        Creates a new PublicKey by copying an old one.

        @param[in] copy The PublicKey to copy from
        */
        PublicKey(const PublicKey &copy) = default;

        /**
        Creates a new PublicKey by moving an old one.

        @param[in] source The PublicKey to move from
        */
        PublicKey(PublicKey &&source) = default;

        /**
        Copies an old PublicKey to the current one.

        @param[in] assign The PublicKey to copy from
        */
        PublicKey &operator =(const PublicKey &assign) = default;

        /**
        Moves an old PublicKey to the current one.

        @param[in] assign The PublicKey to move from
        */
        PublicKey &operator =(PublicKey &&assign) = default;

        /**
        Returns a reference to the underlying BigPolyArray.
        */
        inline auto &data() noexcept
        {
            return pk_;
        }

        /**
        Returns a const reference to the underlying BigPolyArray.
        */
        inline auto &data() const noexcept
        {
            return pk_;
        }

        /**
        Saves the PublicKey to an output stream. The output is in binary format
        and not human-readable. The output stream must have the "binary" flag set.

        @param[in] stream The stream to save the PublicKey to
        */
        inline void save(std::ostream &stream) const
        {
            pk_.save(stream);
        }

        /**
        Loads a PublicKey from an input stream overwriting the current PublicKey.

        @param[in] stream The stream to load the PublicKey from
        */
        inline void load(std::istream &stream)
        {
            pk_.load(stream);
        }

        /**
        Returns a reference to parms_id.
        */
        inline auto &parms_id() noexcept
        {
            return pk_.parms_id();
        }

        /**
        Returns a const reference to parms_id.
        */
        inline auto &parms_id() const noexcept
        {
            return pk_.parms_id();
        }

        /**
        Returns the currently used MemoryPoolHandle.
        */
        inline MemoryPoolHandle pool() const noexcept
        {
            return pk_.pool();
        }

    private:
        Ciphertext pk_;
    };
}
