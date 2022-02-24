#include "seal/ciphertext.h"

using namespace std;
using namespace seal::util;

namespace seal
{
    Ciphertext &Ciphertext::operator =(const Ciphertext &assign)
    {
        // Check for self-assignment
        if (this == &assign)
        {
            return *this;
        }

        // Copy over fields
        parms_id_ = assign.parms_id_;
        is_ntt_form_ = assign.is_ntt_form_;
        scale_ = assign.scale_;

        // Then resize
        resize_internal(assign.size_, assign.poly_modulus_degree_, 
            assign.coeff_mod_count_);

        // Size is guaranteed to be OK now so copy over
        copy(assign.data_.cbegin(), assign.data_.cend(), data_.begin());

        return *this;
    }

    void Ciphertext::reserve_internal(size_type size_capacity, 
        size_type poly_modulus_degree, size_type coeff_mod_count)
    {
        if (size_capacity < SEAL_CIPHERTEXT_SIZE_MIN ||
            size_capacity > SEAL_CIPHERTEXT_SIZE_MAX)
        {
            throw invalid_argument("invalid size_capacity");
        }

        size_type new_data_capacity = 
            size_capacity * poly_modulus_degree * coeff_mod_count;
        size_type new_data_size = min<size_type>(new_data_capacity, data_.size());

        // First reserve, then resize
        data_.reserve(new_data_capacity);
        data_.resize(new_data_size);

        // Set the size and size_capacity
        size_capacity_ = size_capacity;
        size_ = min<size_type>(size_capacity, size_);
        poly_modulus_degree_ = poly_modulus_degree;
        coeff_mod_count_ = coeff_mod_count;
    }

    void Ciphertext::resize_internal(size_type size, 
        size_type poly_modulus_degree, size_type coeff_mod_count)
    {
        if ((size < SEAL_CIPHERTEXT_SIZE_MIN && size != 0) ||
            size > SEAL_CIPHERTEXT_SIZE_MAX)
        {
            throw invalid_argument("invalid size");
        }

        // Resize the data
        size_type new_data_size = size * poly_modulus_degree * coeff_mod_count;
        data_.resize(new_data_size);

        // Set the size parameters
        size_ = size;
        poly_modulus_degree_ = poly_modulus_degree;
        coeff_mod_count_ = coeff_mod_count;
    }

    void Ciphertext::save(ostream &stream) const
    {
        stream.write(reinterpret_cast<const char*>(&parms_id_), sizeof(parms_id_type));
        SEAL_BYTE is_ntt_form_byte = static_cast<SEAL_BYTE>(is_ntt_form_);
        stream.write(reinterpret_cast<const char*>(&is_ntt_form_byte), sizeof(SEAL_BYTE));
        uint64_t size64 = static_cast<uint64_t>(size_);
        stream.write(reinterpret_cast<const char*>(&size64), sizeof(uint64_t));
        uint64_t poly_modulus_degree64 = static_cast<uint64_t>(poly_modulus_degree_);
        stream.write(reinterpret_cast<const char*>(&poly_modulus_degree64), sizeof(uint64_t));
        uint64_t coeff_mod_count64 = static_cast<uint64_t>(coeff_mod_count_);
        stream.write(reinterpret_cast<const char*>(&coeff_mod_count64), sizeof(uint64_t));
        stream.write(reinterpret_cast<const char*>(&scale_), sizeof(double));
        
        // Save the data
        data_.save(stream);
    }

    void Ciphertext::load(istream &stream)
    {
        parms_id_type parms_id{};
        stream.read(reinterpret_cast<char*>(&parms_id), sizeof(parms_id_type));
        SEAL_BYTE is_ntt_form_byte;
        stream.read(reinterpret_cast<char*>(&is_ntt_form_byte), sizeof(SEAL_BYTE));
        uint64_t size64 = 0; stream.read(reinterpret_cast<char*>(&size64), sizeof(uint64_t));
        uint64_t poly_modulus_degree64 = 0;
        stream.read(reinterpret_cast<char*>(&poly_modulus_degree64), sizeof(uint64_t));
        uint64_t coeff_mod_count64 = 0;
        stream.read(reinterpret_cast<char*>(&coeff_mod_count64), sizeof(uint64_t));
        double scale = 0;
        stream.read(reinterpret_cast<char*>(&scale), sizeof(double));

        // Load the data
        IntArray<ct_coeff_type> new_data(data_.pool());
        new_data.load(stream);
        if (new_data.size() != size64 * poly_modulus_degree64 * coeff_mod_count64)
        {
            throw invalid_argument("ciphertext data is invalid");
        }

        // Set values
        parms_id_ = parms_id;
        is_ntt_form_ = (is_ntt_form_byte == SEAL_BYTE(0)) ? false : true;
        size_ = size64;
        poly_modulus_degree_ = poly_modulus_degree64;
        coeff_mod_count_ = coeff_mod_count64;
        scale_ = scale;

        // Set the data
        data_.swap_with(new_data);
    }
}
