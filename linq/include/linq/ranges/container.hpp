#pragma once

#include <type_traits>
#include <iterator>
#include <optional>
#include <cassert>

#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange>
	class container_iterator
	{
	public:

		/// @brief Type definitions
		/// 
		using iterator_category = std::forward_iterator_tag;
		using range_type        = TRange;
		using value_type        = typename range_type::value_type;
		using return_type       = typename range_type::return_type;
		using difference_type   = std::ptrdiff_t;
		using pointer           = value_type *;
		using reference         = value_type &;
		
		inline static constexpr bool returns_reference = std::is_reference_v<return_type>;

	public:

		_NODISCARD_CTOR constexpr container_iterator()
			: has_value(false), range(std::nullopt)
		{
		}

		_NODISCARD_CTOR constexpr container_iterator(range_type range)
			: has_value(false), range(std::move(range))
		{
			this->has_value = this->range.has_value() && this->range->move_next();
		}

		_NODISCARD constexpr return_type operator * () const
		{
			assert(this->has_value);
			assert(this->range.has_value());
			
			return this->range->get_value();
		}

		_NODISCARD constexpr const value_type * operator -> () const
		{
			static_assert(returns_reference, "operator-> requires a range that returns a reference, typically select causes ranges to return values not references");
			return &this->range->get_value();
		}

		_NODISCARD constexpr container_iterator & operator ++ ()
		{
			if(this->has_value && this->range.has_value())
			{
				this->has_value = this->range->move_next();
			}
			
			return *this;
		}

		_NODISCARD constexpr bool operator == (const container_iterator & other) const
		{
			return this->has_value == other.has_value;
		}

		_NODISCARD constexpr bool operator != (const container_iterator & other) const
		{
			return !(*this == other);
		}
	
	private:

		/// @brief member attributes
		/// 
		
		bool                      has_value;
		std::optional<range_type> range;
	};

	template<range_concept TRange>
	class container
	{
	public:

		/// @brief Type definitions
		/// 
		using range_type     = TRange;
		using const_iterator = container_iterator<range_type>;
		
	public:

		_NODISCARD_CTOR constexpr container(range_type range)
			: range(std::move(range))
		{
		}

		_NODISCARD constexpr const_iterator begin() const
		{
			return const_iterator(this->range);
		}

		_NODISCARD constexpr const_iterator end() const
		{
			return const_iterator();
		}

		_NODISCARD constexpr typename const_iterator::difference_type size() const
		{
			return std::distance(this->begin(), this->end());
		}
	
	private:

		range_type range;
		
	};
	
}