#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

template<typename T>
struct custom_allocator
{
	using value_type = T;

	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	template<typename U>
	struct rebind
	{
		using other = custom_allocator<U>;
	};

	custom_allocator() = default;
	~custom_allocator() = default;

	template<typename U>
	custom_allocator(const custom_allocator<U>&)
	{
	}

	T* allocate(std::size_t n)
	{
		auto p = std::malloc(sizeof(T) * n);
		if (!p)
		{
			throw std::bad_alloc();
		}
		return reinterpret_cast<T*>(p);
	}

	void deallocate(T* p, std::size_t n)
	{
		std::free(p);
	}

	template<typename U, typename ...Args>
	void construct(U* p, Args&& ...args)
	{
		new(p) U(std::forward<Args>(args)...);
	}

	void destroy(T* p)
	{
		p->~T();
	}
};

int main(/*int argc, char const *argv[]*/)
{
	auto v = std::vector<int, custom_allocator<int>>();
	v.push_back(5);

	std::cout << v[0] << std::endl;

	auto m = std::map<int, int, std::less<int>, custom_allocator<int>>();
	m[3] = 14;

	std::cout << m[3] << std::endl;

    return 0;
}
