#pragma once

#include <linq/utils/concepts.hpp>

#include <set>

namespace linq
{

	template<range_concept TRange, typename TEnumerable>
	class union_range
	{
	public:

		using enumerable        = std::remove_cvref_t<TEnumerable>;
		using lhs_range_type    = std::remove_cvref_t<TRange>;
		using rhs_range_type    = typename enumerable::range_type;
		using value_type        = std::remove_cvref_t<typename lhs_range_type::value_type>;
		using return_type       = const value_type &;
		using set_type          = std::set<value_type>;
		using set_iterator_type = typename set_type::const_iterator;
	
	public:

		_NODISCARD_CTOR explicit union_range(
			const lhs_range_type & lhs_range,
			const rhs_range_type & rhs_range
		)
			: lhs_range(lhs_range), rhs_range(rhs_range), values(), iterator()
		{}

		_NODISCARD return_type get_value() const
		{
			return *this->iterator;
		}

		_NODISCARD bool move_next()
		{
			while (this->rhs_range.move_next())
			{
				const auto insertionResult = this->values.insert(this->rhs_range.get_value());
				if (insertionResult.second)
				{
					this->iterator = insertionResult.first;
					return true;
				}
			}
			
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
		
		lhs_range_type    lhs_range;
		rhs_range_type    rhs_range;
		set_type          values;
		set_iterator_type iterator;
	};
	
}