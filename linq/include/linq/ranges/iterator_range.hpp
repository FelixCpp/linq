#pragma once

#include <linq/utils/iterator_traits.hpp>

namespace linq
{

	template<typename TIterator>
	class iterator_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using iterator    = typename iterator_traits<TIterator>::iterator;
		using value_type  = typename iterator_traits<TIterator>::value_type;
		using return_type = const value_type &;

		/// <summary>
		/// Constructs an iterator_range
		/// </summary>
		/// <param name="begin">begin of the iterator</param>
		/// <param name="end">end of the iterator</param>
		_NODISCARD_CTOR explicit iterator_range(const iterator & begin, const iterator & end)
			: current(begin), next(begin), end(end)
		{
		}

		/// <summary>
		/// Returns the current value
		/// </summary>
		_NODISCARD return_type get_value() const
		{
			return *this->current;
		}

		/// <summary>
		/// Moves to the next element
		/// </summary>
		/// <returns>True if there is an element left to process</returns>
		_NODISCARD bool move_next()
		{
			if (this->next == this->end)
				return false;

			this->current = this->next;
			++this->next;
			return true;
		}
	
	private:

		/// <summary>
		/// Member data
		/// </summary>
		
		iterator current{};
		iterator next{};
		iterator end{};
		
	};
	
}