#include "seal/randomgen.h"

using namespace std;

namespace seal
{
    /**
    Returns the default random number generator factory. This instance should
    not be destroyed.
    */
    auto UniformRandomGeneratorFactory::default_factory()
        -> const shared_ptr<UniformRandomGeneratorFactory>
    {
        static const shared_ptr<UniformRandomGeneratorFactory>
            default_factory{ new SEAL_DEFAULT_RNG_FACTORY };
        return default_factory;
    }
}
