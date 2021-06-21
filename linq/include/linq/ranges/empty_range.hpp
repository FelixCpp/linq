#pragma once

#include <linq/utils/exceptions.hpp>

namespace linq
{
	
	template<typename TValue>
	class empty_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using value_type  = std::remove_cvref_t<TValue>;
		using return_type = TValue;

	public:

		/// <summary>
		/// throws an invalid_operation_exception since
		/// we have no value to return ever
		/// </summary>
		return_type get_value() const
		{
			throw invalid_operation_exception();
		}

		/// <summary>
		/// returns false
		/// </summary>
		bool move_next()
		{
			return false;
		}
		
	};
}