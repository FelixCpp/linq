#pragma once

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange, typename TPredicate>
	class where_range
	{
	public:

		static_assert(std::is_invocable_v<TPredicate, typename TRange::value_type>, "typeparam TPredicate (where_range) has an invalid format");
		static_assert(std::is_same_v<std::invoke_result_t<TPredicate, typename TRange::value_type>, bool>, "The predicate in where_range must return a boolean");

		/// <summary>
		/// type definitions
		/// </summary>
		using range_type     = std::remove_cvref_t<TRange>;
		using predicate_type = std::remove_cvref_t<TPredicate>;
		using value_type     = typename TRange::value_type;
		using return_type    = typename TRange::return_type;

	public:

		/// <summary>
		/// Constructs a where_range with a range and filters it based on the given predicate
		/// </summary>
		/// <param name="range">the range holding the values to operate on</param>
		/// <param name="predicate">the filter-predicate for each value</param>
		constexpr explicit where_range(
			const range_type & range,
			const predicate_type & predicate
		) : range(range), predicate(predicate)
		{
		}

		/// <summary>
		/// Returns the current value we're iterating over
		/// </summary>
		constexpr return_type get_value() const
		{
			return this->range.get_value();
		}

		/// <summary>
		/// Iterates over the range and returns true if there is
		/// a predicate-match on the current value
		/// </summary>
		constexpr bool move_next()
		{
			while (this->range.move_next())
			{
				if (this->predicate(this->range.get_value()))
					return true;
			}

			return false;
		}
	
	private:

		/// <summary>
		/// Member attributes
		/// </summary>
		
		range_type     range;
		predicate_type predicate;
		
	};
	
}