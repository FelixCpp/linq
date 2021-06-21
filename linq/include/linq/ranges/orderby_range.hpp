#pragma once

#include <list>

#include <linq/ranges/sorting_range.hpp>

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange, typename TSelector>
	class orderby_range : public sorting_range<typename TRange::value_type>
	{
	public:

		static_assert(std::is_invocable_v<TSelector, typename TRange::value_type>, "typeparam TSelector (orderby_range) has an invalid format");
		
		/// <summary>
		/// Type definitions
		/// </summary>
		using range_type         = std::remove_cvref_t<TRange>;
		using selector_type      = std::remove_cvref_t<TSelector>;
		using value_type         = typename range_type::value_type;
		using return_type        = typename range_type::return_type;
		using list_type          = std::list<value_type>;
		using list_iterator_type = typename list_type::const_iterator;

	public:

		/// <summary>
		/// Creates an order_by range
		/// </summary>
		/// <param name="range">the range to sort</param>
		/// <param name="selector">function to select a certain value of the value_type</param>
		/// <param name="ascending">a boolean to indicate the sorting-direction</param>
		constexpr explicit orderby_range(
			const range_type & range,
			const selector_type & selector,
			const bool ascending
		) : range(range), selector(selector), ascending(ascending), values(), iterator()
		{}

		constexpr return_type forward_get_value() const
		{
			return this->range.get_value();
		}

		constexpr bool forward_move_next()
		{
			return this->range.move_next();
		}

		/// <summary>
		/// Returns the current value
		/// </summary>
		constexpr return_type get_value() const
		{
			return *this->iterator;
		}

		/// <summary>
		/// Moves to the next element in the range
		/// </summary>
		constexpr bool move_next()
		{
			if(this->values.empty())
			{
				while (this->range.move_next())
					this->values.push_back(this->range.get_value());

				this->values.sort([this](const value_type & lhs, const value_type & rhs)
				{
					return this->compare_values(lhs, rhs);
				});

				this->iterator = this->values.begin();
			} else
			{
				++this->iterator;
			}
			
			return this->iterator != this->values.end();
		}

		virtual bool compare_values(const value_type & lhs, const value_type & rhs) const override
		{
			if (this->ascending)
				return this->selector(lhs) < this->selector(rhs);

			return this->selector(rhs) < this->selector(lhs);
		}
	
	private:

		range_type         range;
		selector_type      selector;
		bool               ascending;
		list_type          values;
		list_iterator_type iterator;
		
	};
	
}