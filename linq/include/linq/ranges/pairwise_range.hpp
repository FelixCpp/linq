#pragma once

#include <utility>
#include <optional>

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange>
	class pairwise_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using range_type    = std::remove_cvref_t<TRange>;
		using element_type  = typename range_type::value_type;
		using value_type    = std::pair<element_type, element_type>;
		using return_type   = value_type;
		using optional_type = std::optional<element_type>;
	
	public:

		/// <summary>
		/// Constructs a pairwise_range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		constexpr explicit pairwise_range(const range_type & range)
			: range(range), previous(std::nullopt), current(std::nullopt)
		{
		}

		/// <summary>
		/// Returns a pair of the current and the next value
		/// </summary>
		constexpr return_type get_value() const
		{
			return std::make_pair(this->previous.value(), this->current.value());
		}

		/// <summary>
		/// Retrieves the new values
		/// </summary>
		constexpr bool move_next()
		{
			if(!this->previous.has_value())
			{
				if(this->range.move_next())
				{
					this->current = this->range.get_value();
				} else
				{
					return false;
				}
			}

			this->previous = this->current;

			if(this->range.move_next())
			{
				this->current = this->range.get_value();
				return true;
			}

			this->previous.reset();
			this->current.reset();
			return false;
		}
	
	private:

		/// <summary>
		/// Member attributes
		/// </summary>
		
		range_type range;

		optional_type previous;
		optional_type current;
		
	};
	
}