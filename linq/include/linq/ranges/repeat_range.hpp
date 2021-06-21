#pragma once

#include <cstdio>

namespace linq
{

	template<typename TValue>
	class repeat_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using value_type  = std::remove_cvref_t<TValue>;
		using return_type = const TValue &;

	public:
		
		/// <summary>
		/// Constructs a repeating range
		/// </summary>
		/// <param name="value">the value to repeat n times</param>
		/// <param name="repetitions">the number of repetitions</param>
		constexpr explicit repeat_range(const value_type & value, const size_t repetitions)
			: value(value), repetitions(repetitions)
		{
		}

		/// <summary>
		/// Returns the privately stored value
		/// </summary>
		constexpr return_type get_value() const
		{
			return this->value;
		}

		/// <summary>
		/// Steps forward by one element
		/// </summary>
		constexpr bool move_next()
		{
			if(this->repetitions > 0)
			{
				--this->repetitions;
				return true;
			}

			return false;
		}
	
	private:

		value_type value;
		size_t     repetitions;
		
	};

}