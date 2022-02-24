#include "CppUnitTest.h"
#include "seal/intarray.h"
#include "seal/memorymanager.h"
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(IntArrayTest)
    {
    public:
        TEST_METHOD(IntArrayBasics)
        {
            {
                auto pool = MemoryPoolHandle::New();
                MemoryManager::SwitchProfile(new MMProfFixed(pool));
                IntArray<int32_t> arr;
                Assert::IsTrue(arr.begin() == nullptr);
                Assert::IsTrue(arr.end() == nullptr);
                Assert::AreEqual(0ULL, arr.size());
                Assert::AreEqual(0ULL, arr.capacity());
                Assert::IsTrue(arr.empty());

                arr.resize(1);
                Assert::IsFalse(arr.begin() == nullptr);
                Assert::IsFalse(arr.end() == nullptr);
                Assert::IsFalse(arr.begin() == arr.end());
                Assert::AreEqual(1, static_cast<int>(arr.end() - arr.begin()));
                Assert::AreEqual(1ULL, arr.size());
                Assert::AreEqual(1ULL, arr.capacity());
                Assert::IsFalse(arr.empty());
                Assert::AreEqual(0, arr[0]);
                arr.at(0) = 1;
                Assert::AreEqual(1, arr[0]);
                Assert::AreEqual(4, static_cast<int>(pool.alloc_byte_count()));

                arr.reserve(6);
                Assert::IsFalse(arr.begin() == nullptr);
                Assert::IsFalse(arr.end() == nullptr);
                Assert::IsFalse(arr.begin() == arr.end());
                Assert::AreEqual(1, static_cast<int>(arr.end() - arr.begin()));
                Assert::AreEqual(1ULL, arr.size());
                Assert::AreEqual(6ULL, arr.capacity());
                Assert::IsFalse(arr.empty());
                Assert::AreEqual(1, arr[0]);
                Assert::AreEqual(28, static_cast<int>(pool.alloc_byte_count()));

                arr.resize(4);
                Assert::IsFalse(arr.begin() == nullptr);
                Assert::IsFalse(arr.end() == nullptr);
                Assert::IsFalse(arr.begin() == arr.end());
                Assert::AreEqual(4, static_cast<int>(arr.end() - arr.begin()));
                Assert::AreEqual(4ULL, arr.size());
                Assert::AreEqual(6ULL, arr.capacity());
                Assert::IsFalse(arr.empty());
                arr.at(0) = 0;
                arr.at(1) = 1;
                arr.at(2) = 2;
                arr.at(3) = 3;
                Assert::AreEqual(0, arr[0]);
                Assert::AreEqual(1, arr[1]);
                Assert::AreEqual(2, arr[2]);
                Assert::AreEqual(3, arr[3]);
                Assert::AreEqual(28, static_cast<int>(pool.alloc_byte_count()));

                arr.shrink_to_fit();
                Assert::IsFalse(arr.begin() == nullptr);
                Assert::IsFalse(arr.end() == nullptr);
                Assert::IsFalse(arr.begin() == arr.end());
                Assert::AreEqual(4, static_cast<int>(arr.end() - arr.begin()));
                Assert::AreEqual(4ULL, arr.size());
                Assert::AreEqual(4ULL, arr.capacity());
                Assert::IsFalse(arr.empty());
                Assert::AreEqual(0, arr[0]);
                Assert::AreEqual(1, arr[1]);
                Assert::AreEqual(2, arr[2]);
                Assert::AreEqual(3, arr[3]);
                Assert::AreEqual(44, static_cast<int>(pool.alloc_byte_count()));
            }
            {
                auto pool = MemoryPoolHandle::New();
                MemoryManager::SwitchProfile(new MMProfFixed(pool));
                IntArray<uint64_t> arr;
                Assert::IsTrue(arr.begin() == nullptr);
                Assert::IsTrue(arr.end() == nullptr);
                Assert::AreEqual(0ULL, arr.size());
                Assert::AreEqual(0ULL, arr.capacity());
                Assert::IsTrue(arr.empty());

                arr.resize(1);
                Assert::IsFalse(arr.begin() == nullptr);
                Assert::IsFalse(arr.end() == nullptr);
                Assert::IsFalse(arr.begin() == arr.end());
                Assert::AreEqual(1, static_cast<int>(arr.end() - arr.begin()));
                Assert::AreEqual(1ULL, arr.size());
                Assert::AreEqual(1ULL, arr.capacity());
                Assert::IsFalse(arr.empty());
                Assert::AreEqual(0ULL, arr[0]);
                arr.at(0) = 1;
                Assert::AreEqual(1ULL, arr[0]);
                Assert::AreEqual(8, static_cast<int>(pool.alloc_byte_count()));

                arr.reserve(6);
                Assert::IsFalse(arr.begin() == nullptr);
                Assert::IsFalse(arr.end() == nullptr);
                Assert::IsFalse(arr.begin() == arr.end());
                Assert::AreEqual(1, static_cast<int>(arr.end() - arr.begin()));
                Assert::AreEqual(1ULL, arr.size());
                Assert::AreEqual(6ULL, arr.capacity());
                Assert::IsFalse(arr.empty());
                Assert::AreEqual(1ULL, arr[0]);
                Assert::AreEqual(56, static_cast<int>(pool.alloc_byte_count()));

                arr.resize(4);
                Assert::IsFalse(arr.begin() == nullptr);
                Assert::IsFalse(arr.end() == nullptr);
                Assert::IsFalse(arr.begin() == arr.end());
                Assert::AreEqual(4, static_cast<int>(arr.end() - arr.begin()));
                Assert::AreEqual(4ULL, arr.size());
                Assert::AreEqual(6ULL, arr.capacity());
                Assert::IsFalse(arr.empty());
                arr.at(0) = 0;
                arr.at(1) = 1;
                arr.at(2) = 2;
                arr.at(3) = 3;
                Assert::AreEqual(0ULL, arr[0]);
                Assert::AreEqual(1ULL, arr[1]);
                Assert::AreEqual(2ULL, arr[2]);
                Assert::AreEqual(3ULL, arr[3]);
                Assert::AreEqual(56, static_cast<int>(pool.alloc_byte_count()));

                arr.shrink_to_fit();
                Assert::IsFalse(arr.begin() == nullptr);
                Assert::IsFalse(arr.end() == nullptr);
                Assert::IsFalse(arr.begin() == arr.end());
                Assert::AreEqual(4, static_cast<int>(arr.end() - arr.begin()));
                Assert::AreEqual(4ULL, arr.size());
                Assert::AreEqual(4ULL, arr.capacity());
                Assert::IsFalse(arr.empty());
                Assert::AreEqual(0ULL, arr[0]);
                Assert::AreEqual(1ULL, arr[1]);
                Assert::AreEqual(2ULL, arr[2]);
                Assert::AreEqual(3ULL, arr[3]);
                Assert::AreEqual(88, static_cast<int>(pool.alloc_byte_count()));
            }
        }

        TEST_METHOD(SaveLoadIntArray)
        {
            IntArray<int32_t> arr(6, 4);
            arr.at(0) = 0;
            arr.at(1) = 1;
            arr.at(2) = 2;
            arr.at(3) = 3;
            stringstream ss;
            arr.save(ss);
            IntArray<int32_t> arr2;
            arr2.load(ss);

            Assert::AreEqual(arr.size(), arr2.size());
            Assert::AreEqual(arr.size(), arr2.capacity());
            Assert::AreEqual(arr[0], arr2[0]);
            Assert::AreEqual(arr[1], arr2[1]);
            Assert::AreEqual(arr[2], arr2[2]);
            Assert::AreEqual(arr[3], arr2[3]);

            arr.resize(2);
            arr[0] = 5;
            arr[1] = 6;
            arr.save(ss);
            arr2.load(ss);

            Assert::AreEqual(arr.size(), arr2.size());
            Assert::AreEqual(4ULL, arr2.capacity());
            Assert::AreEqual(arr[0], arr2[0]);
            Assert::AreEqual(arr[1], arr2[1]);
        }
    };
}