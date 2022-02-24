#include "CppUnitTest.h"
#include "seal/util/mempool.h"
#include "seal/util/pointer.h"
#include "seal/util/common.h"
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace seal::util;
using namespace std;

namespace SEALTest
{
    namespace util
    {
        TEST_CLASS(MemoryPoolTests)
        {
        public:
            TEST_METHOD(TestMemoryPoolMT)
            {
                {
                    MemoryPoolMT pool;
                    Assert::IsTrue(0LL == pool.pool_count());

                    Pointer<uint64_t> pointer{ pool.get_for_byte_count(bytes_per_uint64 * 0) };
                    Assert::IsFalse(pointer.is_set());
                    pointer.release();
                    Assert::IsTrue(0LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    uint64_t *allocation1 = pointer.get();
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    Assert::IsFalse(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Pointer<uint64_t> pointer2 = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    uint64_t *allocation2 = pointer2.get();
                    Assert::IsFalse(allocation2 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    pointer2.release();
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(allocation2 == pointer.get());
                    pointer2 = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(allocation1 == pointer2.get());
                    Pointer<uint64_t> pointer3 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    pointer.release();
                    pointer2.release();
                    pointer3.release();
                    Assert::IsTrue(2LL == pool.pool_count());

                    Pointer<SEAL_BYTE> pointer4 = pool.get_for_byte_count(1);
                    Pointer<SEAL_BYTE> pointer5 = pool.get_for_byte_count(2);
                    Pointer<SEAL_BYTE> pointer6 = pool.get_for_byte_count(1);
                    pointer4.release();
                    pointer5.release();
                    pointer6.release();
                    Assert::IsTrue(4LL == pool.pool_count());
                }
                {
                    MemoryPoolMT pool;
                    Assert::IsTrue(0LL == pool.pool_count());

                    Pointer<int> pointer{ pool.get_for_byte_count(4 * 0) };
                    Assert::IsFalse(pointer.is_set());
                    pointer.release();
                    Assert::IsTrue(0LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(4 * 2);
                    int *allocation1 = pointer.get();
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(4 * 2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(4 * 1);
                    Assert::IsFalse(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(4 * 2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Pointer<int> pointer2 = pool.get_for_byte_count(4 * 2);
                    int *allocation2 = pointer2.get();
                    Assert::IsFalse(allocation2 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    pointer2.release();
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(4 * 2);
                    Assert::IsTrue(allocation2 == pointer.get());
                    pointer2 = pool.get_for_byte_count(4 * 2);
                    Assert::IsTrue(allocation1 == pointer2.get());
                    Pointer<int> pointer3 = pool.get_for_byte_count(4 * 1);
                    pointer.release();
                    pointer2.release();
                    pointer3.release();
                    Assert::IsTrue(2LL == pool.pool_count());

                    Pointer<SEAL_BYTE> pointer4 = pool.get_for_byte_count(1);
                    Pointer<SEAL_BYTE> pointer5 = pool.get_for_byte_count(2);
                    Pointer<SEAL_BYTE> pointer6 = pool.get_for_byte_count(1);
                    pointer4.release();
                    pointer5.release();
                    pointer6.release();
                    Assert::IsTrue(4LL == pool.pool_count());
                }
                {
                    MemoryPoolMT pool;
                    Assert::IsTrue(0LL == pool.pool_count());

                    Pointer<SEAL_BYTE> pointer = pool.get_for_byte_count(0);
                    Assert::IsFalse(pointer.is_set());
                    pointer.release();
                    Assert::IsTrue(0LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(2);
                    SEAL_BYTE *allocation1 = pointer.get();
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(1);
                    Assert::IsFalse(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Pointer<SEAL_BYTE> pointer2 = pool.get_for_byte_count(2);
                    SEAL_BYTE *allocation2 = pointer2.get();
                    Assert::IsFalse(allocation2 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    pointer2.release();
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(2);
                    Assert::IsTrue(allocation2 == pointer.get());
                    pointer2 = pool.get_for_byte_count(2);
                    Assert::IsTrue(allocation1 == pointer2.get());
                    Pointer<SEAL_BYTE> pointer3 = pool.get_for_byte_count(1);
                    pointer.release();
                    pointer2.release();
                    pointer3.release();
                    Assert::IsTrue(2LL == pool.pool_count());
                }
            }

            TEST_METHOD(PointerTestsMT)
            {
                MemoryPool &pool = *global_variables::global_memory_pool;
                {
                    Pointer<uint64_t> p1;
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p1.get() == nullptr);

                    p1 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    uint64_t *allocation1 = p1.get();
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() != nullptr);

                    p1.release();
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p1.get() == nullptr);

                    p1 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() == allocation1);

                    Pointer<uint64_t> p2;
                    p2.acquire(p1);
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p2.is_set());
                    Assert::IsTrue(p2.get() == allocation1);

                    ConstPointer<uint64_t> cp2;
                    cp2.acquire(p2);
                    Assert::IsFalse(p2.is_set());
                    Assert::IsTrue(cp2.is_set());
                    Assert::IsTrue(cp2.get() == allocation1);
                    cp2.release();

                    Pointer<uint64_t> p3 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    Assert::IsTrue(p3.is_set());
                    Assert::IsTrue(p3.get() == allocation1);

                    Pointer<uint64_t> p4 = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(p4.is_set());
                    uint64_t *allocation2 = p4.get();
                    p3.swap_with(p4);
                    Assert::IsTrue(p3.is_set());
                    Assert::IsTrue(p3.get() == allocation2);
                    Assert::IsTrue(p4.is_set());
                    Assert::IsTrue(p4.get() == allocation1);
                    p3.release();
                    p4.release();
                }
                {
                    Pointer<SEAL_BYTE> p1;
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p1.get() == nullptr);

                    p1 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    SEAL_BYTE *allocation1 = p1.get();
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() != nullptr);

                    p1.release();
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p1.get() == nullptr);

                    p1 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() == allocation1);

                    Pointer<SEAL_BYTE> p2;
                    p2.acquire(p1);
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p2.is_set());
                    Assert::IsTrue(p2.get() == allocation1);

                    ConstPointer<SEAL_BYTE> cp2;
                    cp2.acquire(p2);
                    Assert::IsFalse(p2.is_set());
                    Assert::IsTrue(cp2.is_set());
                    Assert::IsTrue(cp2.get() == allocation1);
                    cp2.release();

                    Pointer<SEAL_BYTE> p3 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    Assert::IsTrue(p3.is_set());
                    Assert::IsTrue(p3.get() == allocation1);

                    Pointer<SEAL_BYTE> p4 = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(p4.is_set());
                    SEAL_BYTE *allocation2 = p4.get();
                    p3.swap_with(p4);
                    Assert::IsTrue(p3.is_set());
                    Assert::IsTrue(p3.get() == allocation2);
                    Assert::IsTrue(p4.is_set());
                    Assert::IsTrue(p4.get() == allocation1);
                    p3.release();
                    p4.release();
                }
            }

            TEST_METHOD(DuplicateIfNeededMT)
            {
                {
                    unique_ptr<uint64_t[]> allocation(new uint64_t[2]);
                    allocation[0] = 0x1234567812345678;
                    allocation[1] = 0x8765432187654321;

                    MemoryPoolMT pool;
                    Pointer<uint64_t> p1 = duplicate_if_needed(allocation.get(), 2, false, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() == allocation.get());
                    Assert::IsTrue(0LL == pool.pool_count());

                    p1 = duplicate_if_needed(allocation.get(), 2, true, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsFalse(p1.get() == allocation.get());
                    Assert::IsTrue(1LL == pool.pool_count());
                    Assert::IsTrue(p1.get()[0] == 0x1234567812345678);
                    Assert::IsTrue(p1.get()[1] == 0x8765432187654321);
                    p1.release();
                }
                {
                    unique_ptr<int64_t[]> allocation(new int64_t[2]);
                    allocation[0] = 0x1234567812345678;
                    allocation[1] = 0x8765432187654321;

                    MemoryPoolMT pool;
                    Pointer<int64_t> p1 = duplicate_if_needed(allocation.get(), 2, false, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() == allocation.get());
                    Assert::IsTrue(0LL == pool.pool_count());

                    p1 = duplicate_if_needed(allocation.get(), 2, true, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsFalse(p1.get() == allocation.get());
                    Assert::IsTrue(1LL == pool.pool_count());
                    Assert::IsTrue(p1.get()[0] == 0x1234567812345678);
                    Assert::IsTrue(p1.get()[1] == 0x8765432187654321);
                    p1.release();
                }
                {
                    unique_ptr<int[]> allocation(new int[2]);
                    allocation[0] = 0x123;
                    allocation[1] = 0x876;

                    MemoryPoolMT pool;
                    Pointer<int> p1 = duplicate_if_needed(allocation.get(), 2, false, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() == allocation.get());
                    Assert::IsTrue(0LL == pool.pool_count());

                    p1 = duplicate_if_needed(allocation.get(), 2, true, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsFalse(p1.get() == allocation.get());
                    Assert::IsTrue(1LL == pool.pool_count());
                    Assert::IsTrue(p1.get()[0] == 0x123);
                    Assert::IsTrue(p1.get()[1] == 0x876);
                    p1.release();
                }
            }

            TEST_METHOD(TestMemoryPoolST)
            {
                {
                    MemoryPoolMT pool;
                    Assert::IsTrue(0LL == pool.pool_count());

                    Pointer<uint64_t> pointer{ pool.get_for_byte_count(bytes_per_uint64 * 0) };
                    Assert::IsFalse(pointer.is_set());
                    pointer.release();
                    Assert::IsTrue(0LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    uint64_t *allocation1 = pointer.get();
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    Assert::IsFalse(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Pointer<uint64_t> pointer2 = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    uint64_t *allocation2 = pointer2.get();
                    Assert::IsFalse(allocation2 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    pointer2.release();
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(allocation2 == pointer.get());
                    pointer2 = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(allocation1 == pointer2.get());
                    Pointer<uint64_t> pointer3 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    pointer.release();
                    pointer2.release();
                    pointer3.release();
                    Assert::IsTrue(2LL == pool.pool_count());

                    Pointer<SEAL_BYTE> pointer4 = pool.get_for_byte_count(1);
                    Pointer<SEAL_BYTE> pointer5 = pool.get_for_byte_count(2);
                    Pointer<SEAL_BYTE> pointer6 = pool.get_for_byte_count(1);
                    pointer4.release();
                    pointer5.release();
                    pointer6.release();
                    Assert::IsTrue(4LL == pool.pool_count());
                }
                {
                    MemoryPoolMT pool;
                    Assert::IsTrue(0LL == pool.pool_count());

                    Pointer<int> pointer{ pool.get_for_byte_count(4 * 0) };
                    Assert::IsFalse(pointer.is_set());
                    pointer.release();
                    Assert::IsTrue(0LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(4 * 2);
                    int *allocation1 = pointer.get();
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(4 * 2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(4 * 1);
                    Assert::IsFalse(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(4 * 2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Pointer<int> pointer2 = pool.get_for_byte_count(4 * 2);
                    int *allocation2 = pointer2.get();
                    Assert::IsFalse(allocation2 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    pointer2.release();
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(4 * 2);
                    Assert::IsTrue(allocation2 == pointer.get());
                    pointer2 = pool.get_for_byte_count(4 * 2);
                    Assert::IsTrue(allocation1 == pointer2.get());
                    Pointer<int> pointer3 = pool.get_for_byte_count(4 * 1);
                    pointer.release();
                    pointer2.release();
                    pointer3.release();
                    Assert::IsTrue(2LL == pool.pool_count());

                    Pointer<SEAL_BYTE> pointer4 = pool.get_for_byte_count(1);
                    Pointer<SEAL_BYTE> pointer5 = pool.get_for_byte_count(2);
                    Pointer<SEAL_BYTE> pointer6 = pool.get_for_byte_count(1);
                    pointer4.release();
                    pointer5.release();
                    pointer6.release();
                    Assert::IsTrue(4LL == pool.pool_count());
                }
                {
                    MemoryPoolMT pool;
                    Assert::IsTrue(0LL == pool.pool_count());

                    Pointer<SEAL_BYTE> pointer = pool.get_for_byte_count(0);
                    Assert::IsFalse(pointer.is_set());
                    pointer.release();
                    Assert::IsTrue(0LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(2);
                    SEAL_BYTE *allocation1 = pointer.get();
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(1LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(1);
                    Assert::IsFalse(allocation1 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    Assert::IsFalse(pointer.is_set());
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(2);
                    Assert::IsTrue(allocation1 == pointer.get());
                    Pointer<SEAL_BYTE> pointer2 = pool.get_for_byte_count(2);
                    SEAL_BYTE *allocation2 = pointer2.get();
                    Assert::IsFalse(allocation2 == pointer.get());
                    Assert::IsTrue(pointer.is_set());
                    pointer.release();
                    pointer2.release();
                    Assert::IsTrue(2LL == pool.pool_count());

                    pointer = pool.get_for_byte_count(2);
                    Assert::IsTrue(allocation2 == pointer.get());
                    pointer2 = pool.get_for_byte_count(2);
                    Assert::IsTrue(allocation1 == pointer2.get());
                    Pointer<SEAL_BYTE> pointer3 = pool.get_for_byte_count(1);
                    pointer.release();
                    pointer2.release();
                    pointer3.release();
                    Assert::IsTrue(2LL == pool.pool_count());
                }
            }

            TEST_METHOD(PointerTestsST)
            {
                MemoryPool &pool = *global_variables::global_memory_pool;
                {
                    Pointer<uint64_t> p1;
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p1.get() == nullptr);

                    p1 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    uint64_t *allocation1 = p1.get();
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() != nullptr);

                    p1.release();
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p1.get() == nullptr);

                    p1 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() == allocation1);

                    Pointer<uint64_t> p2;
                    p2.acquire(p1);
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p2.is_set());
                    Assert::IsTrue(p2.get() == allocation1);

                    ConstPointer<uint64_t> cp2;
                    cp2.acquire(p2);
                    Assert::IsFalse(p2.is_set());
                    Assert::IsTrue(cp2.is_set());
                    Assert::IsTrue(cp2.get() == allocation1);
                    cp2.release();

                    Pointer<uint64_t> p3 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    Assert::IsTrue(p3.is_set());
                    Assert::IsTrue(p3.get() == allocation1);

                    Pointer<uint64_t> p4 = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(p4.is_set());
                    uint64_t *allocation2 = p4.get();
                    p3.swap_with(p4);
                    Assert::IsTrue(p3.is_set());
                    Assert::IsTrue(p3.get() == allocation2);
                    Assert::IsTrue(p4.is_set());
                    Assert::IsTrue(p4.get() == allocation1);
                    p3.release();
                    p4.release();
                }
                {
                    Pointer<SEAL_BYTE> p1;
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p1.get() == nullptr);

                    p1 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    SEAL_BYTE *allocation1 = p1.get();
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() != nullptr);

                    p1.release();
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p1.get() == nullptr);

                    p1 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() == allocation1);

                    Pointer<SEAL_BYTE> p2;
                    p2.acquire(p1);
                    Assert::IsFalse(p1.is_set());
                    Assert::IsTrue(p2.is_set());
                    Assert::IsTrue(p2.get() == allocation1);

                    ConstPointer<SEAL_BYTE> cp2;
                    cp2.acquire(p2);
                    Assert::IsFalse(p2.is_set());
                    Assert::IsTrue(cp2.is_set());
                    Assert::IsTrue(cp2.get() == allocation1);
                    cp2.release();

                    Pointer<SEAL_BYTE> p3 = pool.get_for_byte_count(bytes_per_uint64 * 1);
                    Assert::IsTrue(p3.is_set());
                    Assert::IsTrue(p3.get() == allocation1);

                    Pointer<SEAL_BYTE> p4 = pool.get_for_byte_count(bytes_per_uint64 * 2);
                    Assert::IsTrue(p4.is_set());
                    SEAL_BYTE *allocation2 = p4.get();
                    p3.swap_with(p4);
                    Assert::IsTrue(p3.is_set());
                    Assert::IsTrue(p3.get() == allocation2);
                    Assert::IsTrue(p4.is_set());
                    Assert::IsTrue(p4.get() == allocation1);
                    p3.release();
                    p4.release();
                }
            }

            TEST_METHOD(DuplicateIfNeededST)
            {
                {
                    unique_ptr<uint64_t[]> allocation(new uint64_t[2]);
                    allocation[0] = 0x1234567812345678;
                    allocation[1] = 0x8765432187654321;

                    MemoryPoolMT pool;
                    Pointer<uint64_t> p1 = duplicate_if_needed(allocation.get(), 2, false, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() == allocation.get());
                    Assert::IsTrue(0LL == pool.pool_count());

                    p1 = duplicate_if_needed(allocation.get(), 2, true, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsFalse(p1.get() == allocation.get());
                    Assert::IsTrue(1LL == pool.pool_count());
                    Assert::IsTrue(p1.get()[0] == 0x1234567812345678);
                    Assert::IsTrue(p1.get()[1] == 0x8765432187654321);
                    p1.release();
                }
                {
                    unique_ptr<int64_t[]> allocation(new int64_t[2]);
                    allocation[0] = 0x1234567812345678;
                    allocation[1] = 0x8765432187654321;

                    MemoryPoolMT pool;
                    Pointer<int64_t> p1 = duplicate_if_needed(allocation.get(), 2, false, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() == allocation.get());
                    Assert::IsTrue(0LL == pool.pool_count());

                    p1 = duplicate_if_needed(allocation.get(), 2, true, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsFalse(p1.get() == allocation.get());
                    Assert::IsTrue(1LL == pool.pool_count());
                    Assert::IsTrue(p1.get()[0] == 0x1234567812345678);
                    Assert::IsTrue(p1.get()[1] == 0x8765432187654321);
                    p1.release();
                }
                {
                    unique_ptr<int[]> allocation(new int[2]);
                    allocation[0] = 0x123;
                    allocation[1] = 0x876;

                    MemoryPoolMT pool;
                    Pointer<int> p1 = duplicate_if_needed(allocation.get(), 2, false, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsTrue(p1.get() == allocation.get());
                    Assert::IsTrue(0LL == pool.pool_count());

                    p1 = duplicate_if_needed(allocation.get(), 2, true, pool);
                    Assert::IsTrue(p1.is_set());
                    Assert::IsFalse(p1.get() == allocation.get());
                    Assert::IsTrue(1LL == pool.pool_count());
                    Assert::IsTrue(p1.get()[0] == 0x123);
                    Assert::IsTrue(p1.get()[1] == 0x876);
                    p1.release();
                }
            }
        };
    }
}
