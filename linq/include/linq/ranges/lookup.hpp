#pragma once

#include <list>
#include <map>

#include <linq/utils/concepts.hpp>

#include <linq/enumerable.hpp>

namespace linq
{

	template<typename TKey, typename TValue>
	class outer_lookup_range
	{
		
	public:

		using key_type          = TKey;
		using values_type       = std::list<TValue>;
		using map_type          = std::map<key_type, values_type>;
		using map_iterator_type = typename map_type::const_iterator;
		using value_type        = typename map_type::value_type;
		using return_type       = value_type;

	public:
		
		template<range_concept TRange, typename TKeySelector>
		_NODISCARD outer_lookup_range(TRange range, TKeySelector selector)
			: map(), iterator(), start(true)
		{
			// go through the range
			while (range.move_next())
			{
				// get the value
				const TValue value = range.get_value();

				// extract the key-value using the selector function
				const TKey   key = selector(value);

				// insert the pair into the map
				this->map[key].push_back(value);
			}
		}
		
		/// @brief returns the current value
		/// 
		_NODISCARD return_type get_value() const
		{
			return *this->iterator;
		}

		/// @brief Increments the iterator and returns true if there is an element to process
		/// 
		_NODISCARD bool move_next()
		{
			if (this->start)
			{
				this->iterator = this->map.begin();
				this->start = false;
			} else
			{
				++this->iterator;
			}

			return this->iterator != this->map.end();
		}

		_NODISCARD values_type get_values_for_key(key_type key) const
		{
			return this->map.at(key);
		}
	
	private:

		/// @brief Member attributes
		/// 

		map_type          map;
		map_iterator_type iterator;
		bool              start;

		
	};

	template<typename TKey, typename TValue>
	class inner_lookup_range
	{
	public:

		using key_type        = TKey;
		using value_type      = TValue;
		using values_type     = std::list<value_type>;
		using iterator_type   = typename values_type::const_iterator;
		using return_type     = const value_type &;

	public:

		_NODISCARD_CTOR inner_lookup_range(values_type values)
			: values(std::move(values)), iterator(), start(true)
		{
			
		}

		_NODISCARD return_type get_value() const
		{
			return *this->iterator;
		}

		_NODISCARD bool move_next()
		{
			if (this->start)
			{
				this->iterator = this->values.begin();
				this->start = false;
			} else
			{
				++this->iterator;
			}

			return this->iterator != this->values.end();
		}
	
	private:

		values_type     values;
		iterator_type   iterator;
		bool            start;
		
	};
	
	/// @brief This class behaves like a std::map structure and is used to access certain values through a key or operate on the full range using an enumerable
	/// @tparam TKey The key-type of the lookup structure
	/// @tparam TValue The value-type of the lookup structure
	template<typename TKey, typename TValue>
	class lookup : public enumerable<outer_lookup_range<TKey, TValue>>
	{
	public:

		/// @brief Constructs a lookup
		/// @tparam TRange the range-type
		/// @tparam TKeySelector the function-type for key selection
		/// @param range range instance
		/// @param selector key-selector instance
		template<range_concept TRange, typename TKeySelector>
		_NODISCARD lookup(TRange range, TKeySelector selector)
			: enumerable<outer_lookup_range<TKey, TValue>>(outer_lookup_range<TKey, TValue>(range, selector))
		{}

		_NODISCARD enumerable<inner_lookup_range<TKey, TValue>> operator [] (TKey key) const
		{
			const auto collection = this->get_range().get_values_for_key(key);
			auto range = inner_lookup_range<TKey, TValue>(collection);
			return enumerable<inner_lookup_range<TKey, TValue>>(range);
		}
		
	};
	
}