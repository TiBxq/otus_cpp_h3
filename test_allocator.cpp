#include <gtest/gtest.h>
#include <list>
#include <string>
#include <functional>
#include <vector>

#include "custom_allocator.h"
#include "my_list.h"

TEST(allocator, std_list_with_custom_allocator)
{
	std::list<std::string, custom_allocator<std::string, 3>> test_list;
	test_list.emplace_back("one");
	test_list.emplace_back("two");
	test_list.emplace_back("three");

    ASSERT_TRUE(*test_list.begin() == "one");
	ASSERT_TRUE(*std::next(test_list.begin(), 1) == "two");
	ASSERT_TRUE(*std::next(test_list.begin(), 2) == "three");
}

TEST(allocator, my_list_with_std_allocator)
{
	my_list<std::string> test_list;
	test_list.add("one");
	test_list.add("two");
	test_list.add("three");

	std::vector<std::string> test_elems{"one", "two", "three"};

	int i = 0;
	test_list.forEach([&](const std::string& elem)
	{
		ASSERT_TRUE(elem == test_elems[i]);
		++i;
	});

	ASSERT_EQ(i, 3);
}

TEST(allocator, my_list_with_custom_allocator)
{
	my_list<std::string, custom_allocator<std::string, 3>> test_list;
	test_list.add("one");
	test_list.add("two");
	test_list.add("three");

	std::vector<std::string> test_elems{"one", "two", "three"};

	int i = 0;
	test_list.forEach([&](const std::string& elem)
	{
		ASSERT_TRUE(elem == test_elems[i]);
		++i;
	});

	ASSERT_EQ(i, 3);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}