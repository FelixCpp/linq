#pragma once

#include <linq/utils/concepts.hpp>
#include <linq/utils/exceptions.hpp>

namespace linq
{

	template<range_concept TLhsRange, range_concept TRhsRange>
	class concat_range
	{
	public:

		/// <summary>
		/// iteration states
		/// </summary>
		enum iterating_state
		{
			state_initial,
			state_iterating_lhs,
			state_iterating_rhs,
			state_end
		};

	public:
		
		/// <summary>
		/// Type definitions
		/// </summary>
		using lhs_range_type = std::remove_cvref_t<TLhsRange>;
		using rhs_range_type = std::remove_cvref_t<TRhsRange>;

		using value_type  = typename lhs_range_type::value_type;
		using return_type = typename lhs_range_type::return_type;

	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		/// <param name="lhs_range">the left-hand-side range</param>
		/// <param name="rhs_range">the right-hand-side to concatenate to the end of the left-hand-side range</param>
		constexpr explicit concat_range(
			const lhs_range_type & lhs_range,
			const rhs_range_type & rhs_range
		) : lhs_range(lhs_range), rhs_range(rhs_range), state(state_initial)
		{
		}

		/// <summary>
		/// Determines the current value through the current state
		/// and returns it
		/// </summary>
		constexpr return_type get_value() const
		{
			switch (this->state)
			{
				case state_iterating_lhs: return this->lhs_range.get_value();
				case state_iterating_rhs: return this->rhs_range.get_value();

				default:
					throw invalid_operation_exception("get_value() called on non-iterating concat_range");
			}
		}

		constexpr bool move_next()
		{
			switch (this->state)
			{
				case state_initial:
				{
					this->state = state_iterating_lhs;
					return this->move_next();
				}

				case state_iterating_lhs:
				{
					if (this->lhs_range.move_next())
						return true;

					this->state = state_iterating_rhs;
					return this->move_next();
				}

				case state_iterating_rhs:
				{
					if (this->rhs_range.move_next())
						return true;

					this->state = state_end;
					return false;
				}

				default:
					return false;
			}
		}
	
	private:

		lhs_range_type lhs_range{};
		rhs_range_type rhs_range{};
		iterating_state state{};
		
	};
	
}