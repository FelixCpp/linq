#pragma once

#include <optional>

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept LhsRange, range_concept RhsRange>
	class zip_with_range
	{
	public:

		/// @brief Type definitions
		/// 
		using lhs_range_type = std::remove_cvref_t<LhsRange>;
		using rhs_range_type = std::remove_cvref_t<RhsRange>;
		using lhs_value_type = std::remove_cvref_t<typename lhs_range_type::value_type>;
		using rhs_value_type = std::remove_cvref_t<typename rhs_range_type::value_type>;
		using value_type     = std::pair<lhs_value_type, rhs_value_type>;
		using return_type    = value_type;
	
	public:

		_NODISCARD_CTOR explicit zip_with_range(lhs_range_type lhs_range, rhs_range_type rhs_range)
			: lhs_range(std::move(lhs_range)), rhs_range(std::move(rhs_range)), lhs_value(std::nullopt), rhs_value(std::nullopt)
		{	
		}

		_NODISCARD return_type get_value() const
		{
			return std::make_pair(*this->lhs_value, *this->rhs_value);
		}

		_NODISCARD bool move_next()
		{
			if(this->lhs_range.move_next())
			{
				this->lhs_value = this->lhs_range.get_value();
			} else
			{
				this->lhs_value.reset();
			}

			if (this->rhs_range.move_next())
			{
				this->rhs_value = this->rhs_range.get_value();
			} else
			{
				this->rhs_value.reset();
			}

			return this->lhs_value.has_value() && this->rhs_value.has_value();
		}
	
	private:

		lhs_range_type lhs_range;
		rhs_range_type rhs_range;

		std::optional<lhs_value_type> lhs_value;
		std::optional<rhs_value_type> rhs_value;

	};
	
}