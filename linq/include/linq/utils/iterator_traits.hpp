#pragma once

namespace linq
{

	template<typename TIterator>
	struct iterator_traits
	{
		/// <summary>
		/// Type definition for an iterator
		/// </summary>
		using iterator = std::remove_cvref_t<TIterator>;

		/// <summary>
		/// A method to get the raw value-type of the iterator
		/// through dereferencing the type
		/// </summary>
		static iterator get_iterator();

		/// <summary>
		/// Extract the type of the iterator
		/// </summary>
		using value_type = std::remove_cvref_t<decltype(*get_iterator())>;
	};
	
}