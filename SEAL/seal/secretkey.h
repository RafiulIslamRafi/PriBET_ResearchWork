#pragma once

#include "seal/plaintext.h"
#include <iostream>

namespace seal
{
    /**
    Class to store a secret key.

    @par Thread Safety
    In general, reading from SecretKey is thread-safe as long as no other 
    thread is concurrently mutating it. This is due to the underlying data 
    structure storing the secret key not being thread-safe.

    @see KeyGenerator for the class that generates the secret key.
    @see PublicKey for the class that stores the public key.
    @see RelinKeys for the class that stores the relinearization keys.
    @see GaloisKeys for the class that stores the Galois keys.
    */
    class SecretKey
    {
        friend class KeyGenerator;

    public:
        /**
        Creates an empty secret key.
        */
        SecretKey() = default;

        /**
        Creates a new SecretKey by copying an old one.

        @param[in] copy The SecretKey to copy from
        */
        SecretKey(const SecretKey &copy) = default;

        /**
        Creates a new SecretKey by moving an old one.

        @param[in] source The SecretKey to move from
        */
        SecretKey(SecretKey &&source) = default;

        /**
        Copies an old SecretKey to the current one.

        @param[in] assign The SecretKey to copy from
        */
        SecretKey &operator =(const SecretKey &assign)
        {
            sk_ = assign.sk_;
            return *this;
        }

        /**
        Moves an old SecretKey to the current one.

        @param[in] assign The SecretKey to move from
        */
        SecretKey &operator =(SecretKey &&assign) = default;

        /**
        Returns a reference to the underlying BigPoly.
        */
        inline auto &data() noexcept
        {
            return sk_;
        }

        /**
        Returns a const reference to the underlying BigPoly.
        */
        inline auto &data() const noexcept
        {
            return sk_;
        }

        /**
        Saves the SecretKey to an output stream. The output is in binary format and 
        not human-readable. The output stream must have the "binary" flag set.

        @param[in] stream The stream to save the SecretKey to
        @see load() to load a saved SecretKey.
        */
        inline void save(std::ostream &stream) const
        {
            sk_.save(stream);
        }

        /**
        Loads a SecretKey from an input stream overwriting the current SecretKey.

        @param[in] stream The stream to load the SecretKey from
        @see save() to save a SecretKey.
        */
        inline void load(std::istream &stream)
        {
            sk_.load(stream);
        }

        /**
        Returns a reference to parms_id.

        @see EncryptionParameters for more information about parms_id.
        */
        inline auto &parms_id() noexcept
        {
            return sk_.parms_id();
        }

        /**
        Returns a const reference to parms_id.

        @see EncryptionParameters for more information about parms_id.
        */
        inline auto &parms_id() const noexcept
        {
            return sk_.parms_id();
        }

        /**
        Returns the currently used MemoryPoolHandle.
        */
        inline MemoryPoolHandle pool() const noexcept
        {
            return sk_.pool();
        }

    private:
        Plaintext sk_;
    };
}
