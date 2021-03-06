#include "seal/encryptionparams.h"
#include <limits>

using namespace std;
using namespace seal::util;

namespace seal
{
    void save_encryption_parameters(const EncryptionParameters &parms, ostream &stream)
    {
        uint64_t poly_modulus_degree64 = static_cast<uint64_t>(parms.poly_modulus_degree());
        uint64_t coeff_mod_count64 = static_cast<uint64_t>(parms.coeff_modulus().size());
        auto scheme = parms.scheme();

        stream.write(reinterpret_cast<const char*>(&scheme), sizeof(scheme_type));
        stream.write(reinterpret_cast<const char*>(&poly_modulus_degree64), sizeof(uint64_t));
        stream.write(reinterpret_cast<const char*>(&coeff_mod_count64), sizeof(uint64_t));
        for(const auto &mod : parms.coeff_modulus())
        {
            mod.save(stream);
        }
        parms.plain_modulus().save(stream);
        double noise_standard_deviation = parms.noise_standard_deviation();
        stream.write(reinterpret_cast<const char*>(&noise_standard_deviation), sizeof(double));
    }

    EncryptionParameters load_encryption_parameters(istream &stream)
    {
        // Read the scheme identifier 
        scheme_type scheme;
        stream.read(reinterpret_cast<char*>(&scheme), sizeof(scheme_type));

        EncryptionParameters parms(scheme);

        // Read the poly_modulus_degree
        uint64_t poly_modulus_degree64 = 0;
        stream.read(reinterpret_cast<char*>(&poly_modulus_degree64), sizeof(uint64_t));
        if (poly_modulus_degree64 < SEAL_POLY_MOD_DEGREE_MIN || 
            poly_modulus_degree64 > SEAL_POLY_MOD_DEGREE_MAX)
        {
            throw invalid_argument("poly_modulus_degree is invalid");
        }

        // Read the coeff_modulus size
        uint64_t coeff_mod_count64 = 0;
        stream.read(reinterpret_cast<char*>(&coeff_mod_count64), sizeof(uint64_t));
        if (coeff_mod_count64 > SEAL_COEFF_MOD_COUNT_MAX || 
            coeff_mod_count64 < SEAL_COEFF_MOD_COUNT_MIN)
        {
            throw invalid_argument("coeff_modulus is invalid");
        }
 
        // Read the coeff_modulus
        vector<SmallModulus> coeff_modulus(coeff_mod_count64);
        for(auto &mod : coeff_modulus)
        {
            mod.load(stream);
        }

        // Read the plain_modulus
        SmallModulus plain_modulus;
        plain_modulus.load(stream);

        // Read noise_standard_deviation
        double noise_standard_deviation;
        stream.read(reinterpret_cast<char*>(&noise_standard_deviation), sizeof(double));

        // Supposedly everything worked so set the values of member variables
        parms.set_poly_modulus_degree(poly_modulus_degree64);
        parms.set_coeff_modulus(coeff_modulus);
        parms.set_plain_modulus(plain_modulus);
        parms.set_noise_standard_deviation(noise_standard_deviation);

        return parms;
    }

    void EncryptionParameters::compute_parms_id()
    {
        size_t coeff_mod_count = coeff_modulus_.size();

        size_t total_uint64_count =
            1 + // scheme
            1 + // poly_modulus_degree
            coeff_mod_count +
            plain_modulus_.uint64_count() +
            1; // noise_standard_deviation

        auto param_data(allocate_uint(total_uint64_count, pool_));
        uint64_t *param_data_ptr = param_data.get();

        // Write the scheme identifier
        *param_data_ptr++ = static_cast<uint64_t>(scheme_);

        // Write the poly_modulus_degree. Note that it will always be positive.
        *param_data_ptr++ = static_cast<uint64_t>(poly_modulus_degree_);

        for(const auto &mod : coeff_modulus_)
        {
            *param_data_ptr++ = mod.value();
        }

        set_uint_uint(plain_modulus_.data(), plain_modulus_.uint64_count(), param_data_ptr);
        param_data_ptr += plain_modulus_.uint64_count();

        memcpy(param_data_ptr++, &noise_standard_deviation_, sizeof(double));

        HashFunction::sha3_hash(param_data.get(), total_uint64_count, parms_id_);

        // Did we somehow manage to get a zero block as result? This is reserved for
        // plaintexts to indicate non-NTT-transformed form.
        if (parms_id_ == parms_id_zero)
        {
            throw logic_error("parms_id cannot be zero");
        }
    }
}
