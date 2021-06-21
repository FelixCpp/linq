#pragma once

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange, typename TPredicate>
	class skip_while_range
	{
	public:

		static_assert(std::is_invocable_v<TPredicate, typename TRange::value_type>, "typeparam TPredicate (skip_while_range) has an invalid format");
		static_assert(std::is_same_v<std::invoke_result_t<TPredicate, typename TRange::value_type>, bool>, "The predicate in skip_while_range must return a boolean");

		/// <summary>
		/// Type definitions
		/// </summary>
		using range_type     = std::remove_cvref_t<TRange>;
		using predicate_type = std::remove_cvref_t<TPredicate>;
		using value_type     = typename TRange::value_type;
		using return_type    = typename TRange::return_type;
		
	public:

		/// <summary>
		/// creates a skip_while_range with a predicate which indicates how long
		/// the range should skip any elements
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="predicate">a predicate to indicate whether it should be skipped or not</param>
		constexpr explicit skip_while_range(
			const range_type & range,
			const predicate_type & predicate
		) : range(range), predicate(predicate), skipping(true)
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
			while (this->skipping && this->range.move_next())
			{
				if(!this->predicate(this->range.get_value()))
				{
					this->skipping = false;
					return true;
				}
			}

			return this->range.move_next();
		}

	private:

		range_type     range;
		predicate_type predicate;
		bool           skipping;
		
	};

}