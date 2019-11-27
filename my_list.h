#include <cstdlib>
#include <memory>
#include <functional>

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
			m_allocator.deallocate(ptr, 1);
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

	void forEach(const std::function<void(const T&)>& func)
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