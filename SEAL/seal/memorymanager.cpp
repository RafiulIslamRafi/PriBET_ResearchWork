#include "seal/memorymanager.h"

namespace seal
{
    std::unique_ptr<MMProf>
        MemoryManager::mm_prof_{ new MMProfGlobal };

    std::mutex MemoryManager::switch_mutex_;
}
