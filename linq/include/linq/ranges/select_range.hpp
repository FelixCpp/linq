#pragma once

#include <optional>

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange, typename TTransformation>
	class select_range
	{
	public:

		static_assert(std::is_invocable_v<TTransformation, typename TRange::value_type>, "typeparam TTransformation (select_range) has an invalid format");

		/// <summary>
		/// type definitions
		/// </summary>
		using range_type            = std::remove_cvref_t<TRange>;
		using transformation_type   = std::remove_cvref_t<TTransformation>;
		using raw_value_type        = typename TRange::value_type;
		using transformation_result = std::invoke_result_t<TTransformation, raw_value_type>;
		using value_type            = transformation_result;
		using return_type           = value_type;
		using optional_value_type   = std::optional<transformation_result>;
		
	public:

		/// <summary>
		/// Constructs a select_range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="transformation">the transformation function used for each value of the range</param>
		constexpr explicit select_range(
			const range_type & range,
			const transformation_type & transformation
		) : range(range), transformation(transformation), value(std::nullopt)
		{
		}

		/// <summary>
		/// Returns the current value
		/// </summary>
		constexpr return_type get_value() const
		{
			return *this->value;
		}

		/// <summary>
		/// Goes one value forward in the range
		/// </summary>
		constexpr bool move_next()
		{
			if(this->range.move_next())
			{
				this->value = this->transformation(this->range.get_value());
				return true;
			}

			this->value.reset();
			return false;
		}
	
	private:

		/// <summary>
		/// Member attributes
		/// </summary>
		
		range_type          range;
		transformation_type transformation;
		optional_value_type value;
		
	};
	
}