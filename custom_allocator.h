#include <cstdlib>
#include <typeinfo>

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