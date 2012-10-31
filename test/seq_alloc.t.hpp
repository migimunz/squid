#include <cxxtest/TestSuite.h>
#include <parser/seq_alloc.hpp>

using namespace squid;

class SeqAllocTestSuite : public CxxTest::TestSuite
{
public:
	void test_bins()
	{
		memory_bin bin(16);
		TS_ASSERT(bin.can_allocate(8));
		TS_ASSERT(bin.can_allocate(16));
		TS_ASSERT(!bin.can_allocate(17));

		TS_ASSERT(bin.allocate(8) != NULL);
		TS_ASSERT(bin.allocate(8) != NULL);
		TS_ASSERT(bin.allocate(8) == NULL);
	}

	void test_allocator()
	{
		seq_alloc alloc(8);
		TS_ASSERT(alloc._alloc(5) != NULL);
		TS_ASSERT(alloc._alloc(5) != NULL);
		TS_ASSERT(alloc._alloc(5) != NULL);
		TS_ASSERT(alloc._alloc(5) != NULL);
		TS_ASSERT(alloc._alloc(5) != NULL);
		TS_ASSERT(alloc._alloc(5) != NULL);
		TS_ASSERT_THROWS_ANYTHING(alloc._alloc(32));
	}
};