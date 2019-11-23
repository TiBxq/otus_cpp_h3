#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <typeinfo>
#include <memory>
#include <functional>

//#define ENABLE_LOGGING 1

template<typename T, std::size_t N>
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
		using other = custom_allocator<U, N>;
	};

	custom_allocator()
	{
#ifdef ENABLE_LOGGING
		std::cout << "ctor " << typeid(T).name() << std::endl;
#endif
		m_memory = reinterpret_cast<T*>(std::malloc(sizeof(T) * N));
		m_index = 0;
	}

	~custom_allocator()
	{
#ifdef ENABLE_LOGGING
		std::cout << "dtor " << typeid(T).name() << std::endl;
#endif
		std::free(m_memory);
	}

	template<typename U>
	custom_allocator(const custom_allocator<U, N>&)
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
		auto p = m_memory + m_index;
		m_index += n;
		return p;
	}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

	void deallocate(T* p, std::size_t n)
	{
#ifdef ENABLE_LOGGING
		std::cout << "deallocate: [n = " << n << "] " << typeid(T).name() << std::endl;
#endif
	}

#pragma GCC diagnostic pop

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

private:
	T* m_memory;
	int m_index;
};

template<typename T, typename A = std::allocator<T>>
class my_list
{
public:
	struct my_node
	{
		T _data;
		my_node* _next;

		my_node(T a_data)
			: _data(a_data)
			, _next(nullptr)
		{}
	};

	typedef typename A::template rebind<my_node>::other node_allocator;

	my_list() : m_first(nullptr)
	{}

	~my_list()
	{
		my_node* ptr = m_first;
		while(ptr != nullptr)
		{
			m_allocator.destroy(ptr);
			ptr = ptr->_next;
		}
	}

	void add(T elem)
	{
		auto* newElem = m_allocator.allocate(1);
		m_allocator.construct(newElem, elem);

		if(m_first == nullptr)
		{
			m_first = newElem;
		}
		else
		{
			my_node* ptr = m_first;
			while(ptr->_next != nullptr)
			{
				ptr = ptr->_next;
			}

			ptr->_next = newElem;
		}
	}

	void forEach(std::function<void(const T&)> func)
	{
		my_node* ptr = m_first;
		while(ptr != nullptr)
		{
			func(ptr->_data);
			ptr = ptr->_next;
		}
	}
private:
	my_node* m_first;
	node_allocator m_allocator;
};

int main(/*int argc, char const *argv[]*/)
{
	/*auto m = std::map<int, int, std::less<int>, custom_allocator<std::pair<const int, int>, 10>>();
	for(std::size_t i = 0; i < 10; ++i)
	{
		m[i] = i*i;
	}

	for(std::size_t i = 0; i < 10; ++i)
	{
		std::cout << "map[" << i << "] = " << m[i] << std::endl;
	}*/

	auto c = my_list<int, custom_allocator<int, 10>>();
	for(std::size_t i = 0; i < 10; ++i)
	{
		c.add(i);
	}

	c.forEach([](const int& elem)
	{
		std::cout << elem << std::endl;
	});

	//------
	//std::cout << std::endl << "*** end of main ***" << std::endl << std::endl;
    return 0;
}
