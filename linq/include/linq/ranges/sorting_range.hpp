#pragma once

namespace linq
{
	template<typename TValue>
	class sorting_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using value_type = TValue;

	public:

		/// <summary>
		/// Virtual destructor
		/// </summary>
		virtual ~sorting_range() = default;

		/// <summary>
		/// Compares two values against each other
		/// </summary>
		/// <param name="lhs">the left-hand-side</param>
		/// <param name="rhs">the right-hand-side</param>
		virtual bool compare_values(const value_type & lhs, const value_type & rhs) const = 0;
		
	};
}