#pragma once

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange, typename TPredicate>
	class take_while_range
	{
	public:

		static_assert(std::is_invocable_v<TPredicate, typename TRange::value_type>, "typeparam TPredicate (take_while_range) has an invalid format");
		static_assert(std::is_same_v<std::invoke_result_t<TPredicate, typename TRange::value_type>, bool>, "The predicate in take_while_range must return a boolean");
		
		/// <summary>
		/// Type definitions
		/// </summary>
		using range_type     = std::remove_cvref_t<TRange>;
		using predicate_type = std::remove_cvref_t<TPredicate>;
		using value_type     = typename TRange::value_type;
		using return_type    = typename TRange::return_type;

	public:

		/// <summary>
		/// creates a take_while_range with a predicate which indicates how long
		/// the range should take any elements
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="predicate">a predicate to indicate whether it should be skipped or not</param>
		_NODISCARD_CTOR explicit take_while_range(
			const range_type & range,
			const predicate_type & predicate
		) : range(range), predicate(predicate), taking(false)
		{
		}

		/// <summary>
		/// Returns the current value
		/// </summary>
		_NODISCARD return_type get_value() const
		{
			return this->range.get_value();
		}

		/// <summary>
		/// Moves to the next element
		/// </summary>
		_NODISCARD bool move_next()
		{
			while (!this->taking && this->range.move_next())
			{
				if(this->predicate(this->range.get_value()))
				{
					this->taking = true;
					return true;
				}
			}

			return this->range.move_next();
		}

	private:

		range_type     range;
		predicate_type predicate;
		bool           taking;

	};

}