#include "seal/galoiskeys.h"
#include "seal/util/common.h"
#include <stdexcept>

using namespace std;
using namespace seal::util;

namespace seal
{
    GaloisKeys &GaloisKeys::operator =(const GaloisKeys &assign)
    {
        // Check for self-assignment
        if (this == &assign)
        {
            return *this;
        }

        // Copy over fields
        parms_id_ = assign.parms_id_;
        decomposition_bit_count_ = assign.decomposition_bit_count_;

        // Then copy over keys
        keys_.clear();
        size_t keys_dim1 = assign.keys_.size();
        keys_.reserve(keys_dim1);
        for (size_t i = 0; i < keys_dim1; i++)
        {
            size_t keys_dim2 = assign.keys_[i].size();
            keys_.emplace_back();
            keys_[i].reserve(keys_dim2);
            for (size_t j = 0; j < keys_dim2; j++)
            {
                keys_[i].emplace_back(pool_);
                keys_[i][j] = assign.keys_[i][j];
            }
        }

        return *this;
    }

    void GaloisKeys::save(std::ostream &stream) const
    {
        // Save the parms_id
        stream.write(reinterpret_cast<const char*>(&parms_id_), 
            sizeof(parms_id_type));

        // Save the decomposition bit count
        int32_t decomposition_bit_count32 = 
            static_cast<int32_t>(decomposition_bit_count_);
        stream.write(reinterpret_cast<const char*>(&decomposition_bit_count32), 
            sizeof(int32_t));

        // Save the size of keys_
        uint64_t keys_dim1 = static_cast<uint64_t>(keys_.size());
        stream.write(reinterpret_cast<const char*>(&keys_dim1), sizeof(uint64_t));

        // Now loop again over keys_dim1
        for (size_t index = 0; index < keys_dim1; index++)
        {
            // Save second dimension of keys_
            uint64_t keys_dim2 = static_cast<uint64_t>(keys_[index].size());
            stream.write(reinterpret_cast<const char*>(&keys_dim2), sizeof(uint64_t));

            // Loop over keys_dim2 and save all (or none)
            for (size_t j = 0; j < keys_dim2; j++)
            {
                // Save the key
                keys_[index][j].save(stream);
            }
        }
    }

    void GaloisKeys::load(std::istream &stream)
    {
        // Clear current keys
        keys_.clear();

        // Read the parms_id
        stream.read(reinterpret_cast<char*>(&parms_id_),
            sizeof(parms_id_type));

        // Read the decomposition_bit_count
        int32_t decomposition_bit_count32 = 0;
        stream.read(reinterpret_cast<char*>(&decomposition_bit_count32),
            sizeof(int32_t));
        decomposition_bit_count_ = decomposition_bit_count32;

        // Read in the size of keys_
        uint64_t keys_dim1 = 0;
        stream.read(reinterpret_cast<char*>(&keys_dim1), sizeof(uint64_t));

        // Reserve first for dimension of keys_
        keys_.reserve(keys_dim1);

        // Loop over the first dimension of keys_
        for (size_t index = 0; index < keys_dim1; index++)
        {
            // Read the size of the second dimension
            uint64_t keys_dim2 = 0;
            stream.read(reinterpret_cast<char*>(&keys_dim2), sizeof(uint64_t));

            // Don't resize; only reserve
            keys_.emplace_back();
            keys_.back().reserve(keys_dim2);
            for (size_t j = 0; j < keys_dim2; j++)
            {
                Ciphertext new_key(pool_);
                new_key.load(stream);
                keys_[index].emplace_back(move(new_key));
            }
        }
    }
}
