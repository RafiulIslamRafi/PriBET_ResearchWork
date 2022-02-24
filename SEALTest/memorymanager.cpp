#include "CppUnitTest.h"
#include "seal/util/pointer.h"
#include "seal/memorymanager.h"
#include "seal/util/uintcore.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace seal::util;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(MemoryPoolHandleTest)
    {
    public:
        TEST_METHOD(MemoryPoolHandleConstructAssign)
        {
            MemoryPoolHandle pool;
            Assert::IsFalse(pool);
            pool = MemoryPoolHandle::Global();
            Assert::IsTrue(&static_cast<MemoryPool&>(pool) == global_variables::global_memory_pool.get());
            pool = MemoryPoolHandle::New();
            Assert::IsFalse(&pool.operator seal::util::MemoryPool &() == global_variables::global_memory_pool.get());
            MemoryPoolHandle pool2 = MemoryPoolHandle::New();
            Assert::IsFalse(pool == pool2);

            pool = pool2;
            Assert::IsTrue(pool == pool2);
            pool = MemoryPoolHandle::Global();
            Assert::IsFalse(pool == pool2);
            pool2 = MemoryPoolHandle::Global();
            Assert::IsTrue(pool == pool2);
        }

        TEST_METHOD(MemoryPoolHandleAllocate)
        {
            MemoryPoolHandle pool = MemoryPoolHandle::New();
            Assert::IsTrue(0LL == pool.alloc_byte_count());
            {
                auto ptr(allocate_uint(5, pool));
                Assert::IsTrue(5LL * bytes_per_uint64 == pool.alloc_byte_count());
            }

            pool = MemoryPoolHandle::New();
            Assert::IsTrue(0LL * bytes_per_uint64 == pool.alloc_byte_count());
            {
                auto ptr(allocate_uint(5, pool));
                Assert::IsTrue(5LL * bytes_per_uint64 == pool.alloc_byte_count());

                ptr = allocate_uint(8, pool);
                Assert::IsTrue(13LL * bytes_per_uint64 == pool.alloc_byte_count());

                auto ptr2(allocate_uint(2, pool));
                Assert::IsTrue(15LL * bytes_per_uint64 == pool.alloc_byte_count());
            }
        }
    };
}