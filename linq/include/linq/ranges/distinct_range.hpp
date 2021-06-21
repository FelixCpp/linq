#pragma once

#include <set>

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange>
	class distinct_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using range_type = std::remove_cvref_t<TRange>;

		using value_type  = typename range_type::value_type;
		using return_type = typename range_type::return_type;

		using set_type          = std::set<value_type>;
		using set_iterator_type = typename set_type::const_iterator;

	public:

		/// <summary>
		/// Creates an intersects_range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		constexpr explicit distinct_range(const range_type & range)
			: range(range), values(), iterator()
		{
		}

		/// <summary>
		/// Returns the current value
		/// </summary>
		constexpr return_type get_value() const
		{
			return *this->iterator;
		}

		/// <summary>
		/// Increments the iterator to get the next value as
		/// active one
		/// </summary>
		constexpr bool move_next()
		{
			while (this->range.move_next())
			{
				const auto insertionResult = this->values.insert(this->range.get_value());
				if(insertionResult.second)
				{
					this->iterator = insertionResult.first;
					return true;
				}
			}

			this->values.clear();
			return false;
		}

	private:

		/// <summary>
		/// Member attributes
		/// </summary>

		range_type        range;
		set_type          values;
		set_iterator_type iterator;

	};

}