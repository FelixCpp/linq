#pragma once

#include <list>

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange>
	class reverse_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using range_type         = TRange;
		using value_type         = typename TRange::value_type;
		using return_type        = const value_type &;
		using list_type          = std::list<value_type>;
		using list_iterator_type = typename list_type::const_iterator;

	public:

		/// <summary>
		/// Constructs a reverse_range by storing the range
		/// in a member variable and
		/// reverse it when its needed
		/// </summary>
		/// <param name="range">the range to store</param>
		_NODISCARD_CTOR explicit reverse_range(const range_type & range)
			: range(range), values(), iterator()
		{
		}

		/// <summary>
		/// Returns the current value
		/// </summary>
		_NODISCARD return_type get_value() const
		{
			return *this->iterator;
		}

		/// <summary>
		/// Continues iterating over the range
		/// </summary>
		_NODISCARD bool move_next()
		{
			if(this->values.empty())
			{
				while(this->range.move_next())
				{
					this->values.push_front(this->range.get_value());
				}

				this->iterator = this->values.begin();
			}
			else
			{	
				++this->iterator;
			}
			
			return this->iterator != this->values.end();
		}
	
	private:

		/// <summary>
		/// Member attributes
		/// </summary>
		range_type         range;
		list_type          values;
		list_iterator_type iterator;
		
	};
	
}