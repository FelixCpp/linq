#pragma once

#include <linq/utils/concepts.hpp>

#include <map>

namespace linq
{

	template<
		range_concept TRange,
		typename TEnumerable,
		typename TLhsIdSelection,
		typename TRhsIdSelection,
		typename TJoinSelection
	>
	class join_range
	{
	public:

		static_assert(std::is_invocable_v<TLhsIdSelection, typename TRange::value_type>, "TLhsIdSelection (join_range) has an invalid format!");
		static_assert(std::is_invocable_v<TRhsIdSelection, typename TEnumerable::range_type::value_type>, "TRhsIdSelection (join_range) has an invalid format!");
		static_assert(std::is_invocable_v<TJoinSelection, typename TRange::value_type, typename TEnumerable::range_type::value_type>, "TJoinSelection (join_range) has an invalid format!");
		
		/// <summary>
		/// Type definitions
		/// </summary>
		using enumerable            = std::remove_cvref_t<TEnumerable>;
		using lhs_range_type        = std::remove_cvref_t<TRange>;
		using rhs_range_type        = typename enumerable::range_type;
		using lhs_id_selection_type = std::remove_cvref_t<TLhsIdSelection>;
		using rhs_id_selection_type = std::remove_cvref_t<TRhsIdSelection>;
		using join_selection_type   = std::remove_cvref_t<TJoinSelection>;
		using lhs_value_type        = typename lhs_range_type::value_type;
		using rhs_value_type        = typename rhs_range_type::value_type;
		using lhs_id_type           = std::invoke_result_t<lhs_id_selection_type, lhs_value_type>;
		using rhs_id_type           = std::invoke_result_t<rhs_id_selection_type, rhs_value_type>;
		using join_result           = std::invoke_result_t<join_selection_type, lhs_value_type, rhs_value_type>;
		using map_type              = std::multimap<rhs_id_type, rhs_value_type>;
		using map_iterator_type     = typename map_type::const_iterator;
		
		using value_type  = join_result;
		using return_type = value_type;

	public:
		
		constexpr explicit join_range(
			const lhs_range_type & lhs_range,
			const rhs_range_type & rhs_range,
			const lhs_id_selection_type & lhs_id_selection,
			const rhs_id_selection_type & rhs_id_selection,
			const join_selection_type & join_selection
		) : lhs_range(lhs_range),
			rhs_range(rhs_range),
			lhs_id_selection(lhs_id_selection),
			rhs_id_selection(rhs_id_selection),
			join_selection(join_selection),
			start(true),
			map(),
			current()
		{
		}

		constexpr return_type get_value() const
		{
			return this->join_selection(this->lhs_range.get_value(), this->current->second);
		}

		constexpr bool move_next()
		{
			if(this->start)
			{
				this->start = false;
				while(this->rhs_range.move_next())
				{
					const auto value = this->rhs_range.get_value();
					const auto key   = this->rhs_id_selection(value);
					this->map.insert(std::make_pair(key, value));
				}

				this->current = this->map.end();
				if (this->map.empty())
				{		
					return false;
				}
			}

			if(this->current != this->map.end())
			{
				const auto previous = this->current;
				++this->current;

				if(this->current != this->map.end() && !(previous->first < this->current->first))
				{
					return true;
				}
			}

			while (this->lhs_range.move_next())
			{
				const auto value = this->lhs_range.get_value();
				const auto key   = this->lhs_id_selection(value);
				this->current    = this->map.find(key);

				if(this->current != this->map.end())
				{
					return true;
				}
			}

			return false;
		}
	
	private:

		/// <summary>
		/// Member attributes
		/// </summary>
		
		lhs_range_type        lhs_range;
		rhs_range_type        rhs_range;
		lhs_id_selection_type lhs_id_selection;
		rhs_id_selection_type rhs_id_selection;
		join_selection_type   join_selection;
		
		bool              start;
		map_type          map;
		map_iterator_type current;
		
	};
	
}