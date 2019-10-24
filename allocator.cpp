#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <typeinfo>

#define ENABLE_LOGGING 1

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

	custom_allocator()
	{
#ifdef ENABLE_LOGGING
		std::cout << "ctor " << typeid(T).name() << std::endl;
#endif
	}

	~custom_allocator()
	{
#ifdef ENABLE_LOGGING
		std::cout << "dtor " << typeid(T).name() << std::endl;
#endif
	}

	template<typename U>
	custom_allocator(const custom_allocator<U>&)
	{
#ifdef ENABLE_LOGGING
		std::cout << "copy ctor " << typeid(T).name() << std::endl;
#endif
	}

	T* allocate(std::size_t n)
	{
#ifdef ENABLE_LOGGING
		std::cout << "allocate: [n = " << n << "] " << typeid(T).name() << std::endl;
#endif

		auto p = std::malloc(sizeof(T) * n);
		if (!p)
		{
			throw std::bad_alloc();
		}
		return reinterpret_cast<T*>(p);
	}

	void deallocate(T* p, std::size_t n)
	{
#ifdef ENABLE_LOGGING
		std::cout << "deallocate: [n = " << n << "] " << typeid(T).name() << std::endl;
#endif

		std::free(p);
	}

	template<typename U, typename ...Args>
	void construct(U* p, Args&& ...args)
	{
#ifdef ENABLE_LOGGING
		std::cout << "construct " << typeid(T).name() << std::endl;
#endif

		new(p) U(std::forward<Args>(args)...);
	}

	void destroy(T* p)
	{
#ifdef ENABLE_LOGGING
		std::cout << "destroy " << typeid(T).name() << std::endl;
#endif

		p->~T();
	}
};

int main(/*int argc, char const *argv[]*/)
{
	std::cout << "vector: " << std::endl;

	auto v = std::vector<int, custom_allocator<int>>();
	v.push_back(5);

	std::cout << v[0] << std::endl;
	//------
	std::cout << "map: " << std::endl;

	auto m = std::map<int, int, std::less<int>, custom_allocator<std::pair<const int, int>>>();
	m[3] = 14;

	std::cout << m[3] << std::endl;

	//------
	std::cout << std::endl << "*** end of main ***" << std::endl << std::endl;
    return 0;
}
