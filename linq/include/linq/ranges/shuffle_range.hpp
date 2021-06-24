#pragma once

#include <vector>
#include <random>
#include <algorithm>

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange>
	class shuffle_range
	{
	public:

		/// <summary>
		/// Type definitions
		/// </summary>
		using range_type         = std::remove_cvref_t<TRange>;
		using value_type         = typename range_type::value_type;
		using return_type        = const value_type &;
		using list_type          = std::vector<value_type>;
		using list_iterator_type = typename list_type::const_iterator;

	public:

		/// <summary>
		/// Constructs a shuffle range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		_NODISCARD_CTOR explicit shuffle_range(const range_type & range)
			: range(range)
		{}

		_NODISCARD return_type get_value() const
		{
			return *this->iterator;
		}

		_NODISCARD bool move_next()
		{
			if(this->values.empty())
			{
				while(this->range.move_next())
				{
					this->values.push_back(this->range.get_value());
				}

				std::random_device random_device;
				std::mt19937 mersenne_twister(random_device());
				std::shuffle(this->values.begin(), this->values.end(), mersenne_twister);

				this->iterator = this->values.begin();
			} else
			{
				++this->iterator;
			}
			
			return this->iterator != this->values.end();
		}
	
	private:

		/// <summary>
		/// Member attributes
		/// </summary>
		range_type         range;
		list_type          values;
		list_iterator_type iterator;
	};
	
}