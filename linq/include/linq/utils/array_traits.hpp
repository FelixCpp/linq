#pragma once

namespace linq
{

	template<typename>
	struct array_traits;
	
	template<typename TValue, int ArraySize>
	struct array_traits<TValue[ArraySize]>
	{
		/// <summary>
		/// Extract the values from
		/// the template arguments
		/// </summary>
		
		using value_type = TValue;
		using iterator   = const TValue *;
	};
	
}