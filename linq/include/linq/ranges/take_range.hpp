#pragma once

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange>
	class take_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using range_type  = std::remove_cvref_t<TRange>;
		using value_type  = typename TRange::value_type;
		using return_type = typename TRange::return_type;
		using size_type   = std::size_t;

	public:

		/// <summary>
		/// creates a take_range with a number
		/// of how many elements the range should take at
		/// the front
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="count">the number of elements to take starting from the first element</param>
		constexpr explicit take_range(const range_type & range, const size_type count)
			: range(range), remaining(count)
		{
		}

		/// <summary>
		/// Returns the current value
		/// </summary>
		constexpr return_type get_value() const
		{
			return this->range.get_value();
		}

		/// <summary>
		/// Moves to the next element
		/// </summary>
		constexpr bool move_next()
		{
			while (this->remaining > 0 && this->range.move_next())
			{
				--this->remaining;
				return true;
			}

			return false;
		}

	private:

		range_type range;
		size_type  remaining;

	};

}