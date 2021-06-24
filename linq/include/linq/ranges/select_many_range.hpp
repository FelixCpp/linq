#pragma once

#include <optional>

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<typename TRange, typename TEnumerableSelection>
	class select_many_range
	{
	public:

		static_assert(std::is_invocable_v<TEnumerableSelection, typename TRange::value_type>, "TEnumerableSelection (select_many_range) has an invalid format!");

		/// <summary>
		/// Type definitions
		/// </summary>
		using range_type           = std::remove_cvref_t<TRange>;
		using enumerable_selection = std::remove_cvref_t<TEnumerableSelection>;
		using selection_result     = std::invoke_result_t<enumerable_selection, typename range_type::value_type>;
		using enumerable_range     = typename selection_result::range_type;
		using value_type           = typename enumerable_range::value_type;
		using return_type          = typename enumerable_range::return_type;
		using optional_range_type  = std::optional<enumerable_range>;
	
	public:
		
		_NODISCARD_CTOR explicit select_many_range(
			const range_type & range,
			const enumerable_selection & selection
		) : range(range), selection(selection)
		{
		}

		_NODISCARD return_type get_value() const
		{
			return this->current_range->get_value();
		}

		_NODISCARD bool move_next()
		{
			if(this->current_range.has_value() && this->current_range->move_next())
			{
				return true;
			}

			if(this->range.move_next())
			{
				this->current_range = this->selection(this->range.get_value()).to_range();
				return this->current_range->move_next();
			}

			this->current_range.reset();
			return false;
		}

	private:

		range_type           range;
		enumerable_selection selection;
		optional_range_type  current_range;
		
	};
	
}