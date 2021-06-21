#pragma once

#include <optional>

namespace linq
{

	template<typename TValue>
	class increment_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using value_type    = std::remove_cvref_t<TValue>;
		using return_type   = const TValue &;
		
	public:

		/// <summary>
		/// Constructs an increment_range
		/// based on a start, end and increment
		/// values
		/// </summary>
		/// <param name="start">inclusive start value</param>
		/// <param name="end">inclusive end value</param>
		/// <param name="increment">the value to add in every iteration</param>
		constexpr explicit increment_range(
			const value_type & start,
			const value_type & end,
			const value_type & increment
		) : current(start - increment), end(end), increment(increment)
		{
		}

		/// <summary>
		/// Returns the current value
		/// </summary>
		constexpr return_type get_value() const
		{
			return this->current;
		}

		/// <summary>
		/// Increments the private value by
		/// the increment and stops if the value is
		/// above the end
		/// </summary>
		constexpr bool move_next()
		{
			if(this->current >= this->end)
			{
				return false;
			}

			this->current += this->increment;
			return true;
		}
	
	private:

		value_type current;
		value_type end;
		value_type increment;
		
	};
	
}