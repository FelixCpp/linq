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
		using range_type  = TRange;
		using value_type  = typename TRange::value_type;
		using return_type = typename TRange::return_type;
		using list_type   = std::list<value_type>;

	public:

		/// <summary>
		/// Constructs a reverse_range by storing the range
		/// in a member variable and
		/// reverse it when its needed
		/// </summary>
		/// <param name="range">the range to store</param>
		constexpr explicit reverse_range(const range_type & range)
			: range(range), values()
		{
		}

		/// <summary>
		/// Returns the current value
		/// </summary>
		constexpr return_type get_value() const
		{
			return this->values.front();
		}

		/// <summary>
		/// Continues iterating over the range
		/// </summary>
		constexpr bool move_next()
		{
			if(this->values.empty())
			{
				while(this->range.move_next())
				{
					this->values.push_front(this->range.get_value());
				}
			}
			else
			{	
				this->values.pop_front();
			}
			
			return !this->values.empty();
		}
	
	private:

		/// <summary>
		/// Member attributes
		/// </summary>
		range_type range;
		list_type  values;
		
	};
	
}