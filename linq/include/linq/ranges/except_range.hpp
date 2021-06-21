#pragma once

#include <set>

#include <linq/utils/concepts.hpp>

namespace linq
{
	template<range_concept TLhsRange, range_concept TRhsRange>
	class except_range
	{
	public:
		
		/// <summary>
		/// Type definitions
		/// </summary>
		using lhs_range_type    = std::remove_cvref_t<TLhsRange>;
		using rhs_range_type    = std::remove_cvref_t<TRhsRange>;
		using value_type        = typename lhs_range_type::value_type;
		using return_type       = typename lhs_range_type::return_type;
		using set_type          = std::set<value_type>;
		using set_iterator_type = typename set_type::const_iterator;
	
	public:

		/// <summary>
		/// Constructs an except_range
		/// </summary>
		/// <param name="lhs_range">original range</param>
		/// <param name="rhs_range">range holding the values to ignore</param>
		constexpr explicit except_range(
			const lhs_range_type & lhs_range,
			const rhs_range_type & rhs_range
		) : lhs_range(lhs_range), rhs_range(rhs_range), values()//, iterator()
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
		/// Inserts the next element in each call
		/// </summary>
		constexpr bool move_next()
		{
			if(this->values.empty())
			{
				// insert all illegal values
				while (this->rhs_range.move_next())
					this->values.insert(this->rhs_range.get_value());
			}

			// insert a new legal value step by step
			while(this->lhs_range.move_next())
			{
				const auto insertionResult = this->values.insert(this->lhs_range.get_value());
				if(insertionResult.second)
				{
					this->iterator = insertionResult.first;
					return true;
				}
			}

			return false;
		}
	
	private:

		/// <summary>
		/// Member attributes
		/// </summary>
		
		lhs_range_type lhs_range{};
		rhs_range_type rhs_range{};

		set_type          values{};
		set_iterator_type iterator{};
		
	};
}
