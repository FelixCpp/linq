#pragma once

#include <set>

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TLhsRange, range_concept TRhsRange>
	class intersect_with_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using lhs_range_type    = std::remove_cvref_t<TLhsRange>;
		using rhs_range_type    = std::remove_cvref_t<TRhsRange>;
		using value_type        = std::remove_cvref_t<typename lhs_range_type::value_type>;
		using return_type       = const value_type &;
		using set_type          = std::set<value_type>;
		using set_iterator_type = typename set_type::const_iterator;
	
	public:

		/// <summary>
		/// Creates an intersect_with_range
		/// </summary>
		/// <param name="lhs_range">the left-hand-side range</param>
		/// <param name="rhs_range">the right-hand-side range</param>
		_NODISCARD_CTOR explicit intersect_with_range(
			const lhs_range_type & lhs_range,
			const rhs_range_type & rhs_range
		) : lhs_range(lhs_range), rhs_range(rhs_range), values(), iterator()
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
		/// Increments the iterator to get the next value as
		/// active one
		/// </summary>
		_NODISCARD bool move_next()
		{
			if(this->values.empty())
			{
				// insert the original range
				while (this->lhs_range.move_next())
				{
					this->values.insert(this->lhs_range.get_value());
				}
			}

			while (this->rhs_range.move_next())
			{
				const auto insertionResult = this->values.insert(this->rhs_range.get_value());
				if(!insertionResult.second)
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
		
		lhs_range_type lhs_range;
		rhs_range_type rhs_range;

		set_type          values;
		set_iterator_type iterator;
		
	};
	
}