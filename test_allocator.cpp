#include <gtest/gtest.h>

TEST(allocator, allocator_test1)
{
    ASSERT_TRUE(true);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}