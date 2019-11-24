#include <cassert>
#include <iostream>
#include <map>

#include "custom_allocator.h"
#include "my_list.h"

int fact(int num)
{
	return num == 0 ? 1 : num * fact(num - 1);
}

int main(/*int argc, char const *argv[]*/)
{
	// std::map with default allocator
	{
		auto m = std::map<int, int>();
		for(std::size_t i = 0; i < 10; ++i)
		{
			m[i] = fact(i);
		}
	}

	// std::map with custom allocator
	{
		auto m = std::map<int, int, std::less<int>, custom_allocator<std::pair<const int, int>, 10>>();
		for(std::size_t i = 0; i < 10; ++i)
		{
			m[i] = fact(i);
		}

		for(std::size_t i = 0; i < 10; ++i)
		{
			std::cout << i << " " << m[i] << std::endl;
		}
	}

	// custom container with default allocator
	{
		auto c = my_list<int>();
		for(std::size_t i = 0; i < 10; ++i)
		{
			c.add(i);
		}
	}

	// custom container with custom allocator
	{
		auto c = my_list<int, custom_allocator<int, 10>>();
		for(std::size_t i = 0; i < 10; ++i)
		{
			c.add(i);
		}

		c.forEach([](const int& elem)
		{
			std::cout << elem << std::endl;
		});
	}

	//------
	//std::cout << std::endl << "*** end of main ***" << std::endl << std::endl;
    return 0;
}
