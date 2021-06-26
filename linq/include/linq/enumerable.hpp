#pragma once

#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <stack>

#include <linq/ranges/iterator_range.hpp>
#include <linq/ranges/repeat_range.hpp>
#include <linq/ranges/empty_range.hpp>
#include <linq/ranges/increment_range.hpp>

#include <linq/ranges/except_range.hpp>
#include <linq/ranges/concat_range.hpp>
#include <linq/ranges/where_range.hpp>
#include <linq/ranges/select_range.hpp>
#include <linq/ranges/intersect_with_range.hpp>
#include <linq/ranges/distinct_range.hpp>
#include <linq/ranges/skip_range.hpp>
#include <linq/ranges/take_range.hpp>
#include <linq/ranges/skip_while_range.hpp>
#include <linq/ranges/take_while_range.hpp>
#include <linq/ranges/reverse_range.hpp>
#include <linq/ranges/orderby_range.hpp>
#include <linq/ranges/thenby_range.hpp>
#include <linq/ranges/select_many_range.hpp>
#include <linq/ranges/pairwise_range.hpp>
#include <linq/ranges/join_range.hpp>
#include <linq/ranges/union_range.hpp>
#include <linq/ranges/shuffle_range.hpp>
#include <linq/ranges/zip_with_range.hpp>

#include <linq/utils/array_traits.hpp>
#include <linq/utils/concepts.hpp>

namespace linq
{

	template<typename TKey, typename TValue>
	class lookup;
	
	template<range_concept TRange>
	class enumerable
	{
	public:
		
		/// <summary>
		/// type definitions
		/// </summary>
		using range_type  = TRange;
		using value_type  = typename TRange::value_type;
		using return_type = typename TRange::return_type;
	
	private:

		template<typename TLambda, typename TResult>    inline static constexpr bool is_lambda    = std::is_invocable_v<TLambda, value_type> && std::is_same_v<std::invoke_result_t<TLambda, value_type>, TResult>;
		template<typename TPredicate>                   inline static constexpr bool is_predicate = is_lambda<TPredicate, bool>;
		template<typename TAction>                      inline static constexpr bool is_action    = is_lambda<TAction, void>;
		
	public:

		/// <summary>
		/// Constructs an enumerable with
		/// a given range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		_NODISCARD_CTOR explicit enumerable(const range_type & range)
			: range(range)
		{
		}

		/// <summary>
		/// Returns the range of the enumerable
		/// </summary>
		_NODISCARD const range_type & to_range() const
		{
			return this->range;
		}
		
		/// <summary>
		/// Creates a new enumerable which ignores the values
		/// stored in the except_range
		/// </summary>
		/// <param name="collection">an enumerable holding each value to ignore</param>
		template<range_concept TExceptRange>
		_NODISCARD enumerable<except_range<range_type, TExceptRange>> except(const enumerable<TExceptRange> & collection) const
		{
			return enumerable<except_range<range_type, TExceptRange>>(
				except_range<range_type, TExceptRange>(this->range, collection.to_range())
			);
		}

		/// <summary>
		/// Concatenates two range
		/// </summary>
		/// <param name="collection">an enumerable holding the range to concatenate behind the existing range</param>
		/// <returns>an enumerable holding values from the previous and the concatenated ranges</returns>
		template<range_concept TConcatRange>
		_NODISCARD enumerable<concat_range<range_type, TConcatRange>> concat(const enumerable<TConcatRange> & collection) const
		{
			return enumerable<concat_range<range_type, TConcatRange>>(
				concat_range<range_type, TConcatRange>(this->range, collection.to_range())
			);
		}

		/// <summary>
		/// Iterates over the range and filter values based on
		/// the given predicate
		/// </summary>
		/// <param name="predicate">the predicate which checks if the value gets filtered or stays in the range</param>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		_NODISCARD enumerable<where_range<range_type, TPredicate>> where(const TPredicate & predicate) const
		{
			return enumerable<where_range<range_type, TPredicate>>(
				where_range<range_type, TPredicate>(this->range, predicate)
			);
		}

		/// <summary>
		/// Transforms the original value into anything else
		/// </summary>
		/// <typeparam name="TLambda">the lambda to invoke</typeparam>
		/// <typeparam name="TResult">the result type</typeparam>
		/// <param name="transformation">the transformation for each value</param>
		template<typename TLambda, typename TResult = std::invoke_result_t<TLambda, value_type>>
		_NODISCARD enumerable<select_range<range_type, TLambda>> select(const TLambda & transformation) const
		{
			return enumerable<select_range<range_type, TLambda>>(
				select_range<range_type, TLambda>(this->range, transformation)
			);
		}

		/// <summary>
		/// Combines two ranges together and returns an enumerable
		/// of the values which are duplicated in both ranges
		/// </summary>
		/// <param name="collection">an enumerable holding the range to compare against</param>
		/// <returns>an enumerable holding values which both ranges contain</returns>
		template<range_concept TIntersectsRange>
		_NODISCARD enumerable<intersect_with_range<range_type, TIntersectsRange>> intersect_with(const enumerable<TIntersectsRange> & collection) const
		{
			return enumerable<intersect_with_range<range_type, TIntersectsRange>>(
				intersect_with_range<range_type, TIntersectsRange>(this->range, collection.to_range())
			);
		}

		/// <summary>
		/// Removes all duplicates from the range
		/// </summary>
		_NODISCARD enumerable<distinct_range<range_type>> distinct() const
		{
			return enumerable<distinct_range<range_type>>(
				distinct_range<range_type>(this->range)
			);
		}

		/// <summary>
		/// Skips a certain amount of elements at the front
		/// of the range
		/// </summary>
		/// <param name="count">the number of elements to skip</param>
		_NODISCARD enumerable<skip_range<range_type>> skip(size_t count) const
		{
			return enumerable<skip_range<range_type>>(
				skip_range<range_type>(this->range, count)
			);
		}

		/// <summary>
		/// Takes a certain amount of elements at the front
		/// of the range
		/// </summary>
		/// <param name="count">the number of elements to take</param>
		_NODISCARD enumerable<take_range<range_type>> take(size_t count) const
		{
			return enumerable<take_range<range_type>>(
				take_range<range_type>(this->range, count)
			);
		}

		/// <summary>
		/// Skips as long as the predicate returns
		/// true on these elements
		/// </summary>
		/// <param name="predicate">the predicate to check against each value</param>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		_NODISCARD enumerable<skip_while_range<range_type, TPredicate>> skip_while(const TPredicate & predicate) const
		{
			return enumerable<skip_while_range<range_type, TPredicate>>(
				skip_while_range<range_type, TPredicate>(this->range, predicate)
			);
		}

		/// <summary>
		/// Takes as long as the predicate returns
		/// true on these elements
		/// </summary>
		/// <param name="predicate">the predicate to check against each value</param>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		_NODISCARD enumerable<take_while_range<range_type, TPredicate>> take_while(const TPredicate & predicate) const
		{
			return enumerable<take_while_range<range_type, TPredicate>>(
				take_while_range<range_type, TPredicate>(this->range, predicate)
			);
		}

		/// <summary>
		/// Reverses the range
		/// </summary>
		_NODISCARD enumerable<reverse_range<range_type>> reverse()
		{
			return enumerable<reverse_range<range_type>>(
				reverse_range<range_type>(this->range)
			);
		}

		/// <summary>
		/// creates a list based on the values of the range
		/// </summary>
		_NODISCARD std::list<value_type> to_list() const
		{
			range_type copy = this->range;
			
			std::list<value_type> values;

			while (copy.move_next())
			{
				values.push_back(copy.get_value());
			}

			return values;
		}

		/// <summary>
		/// creates a vector based on the values of the range
		/// </summary>
		_NODISCARD std::vector<value_type> to_vector(size_t capacity = 16) const
		{
			range_type copy = this->range;

			std::vector<value_type> values;
			values.reserve(capacity);
			
			while (copy.move_next())
			{
				values.push_back(copy.get_value());
			}

			return values;
		}

		/// <summary>
		/// Iterates through the range and performs an action
		/// for each value
		/// </summary>
		/// <param name="action">the action to perform for each value</param>
		template<typename TAction, typename = std::enable_if_t<is_action<TAction>>>
		_NODISCARD void for_each(const TAction & action) const
		{
			range_type copy = this->range;

			while (copy.move_next())
			{
				action(copy.get_value());
			}
		}

		/// <summary>
		/// Iterates through the range and performs an action
		/// for each value
		/// </summary>
		/// <param name="action">the action to perform for each value</param>
		template<typename TAction, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TAction, value_type, size_t>, void>>>
		_NODISCARD void indexed_for_each(const TAction & action) const
		{
			range_type copy = this->range;
			
			for(size_t index = 0; copy.move_next(); index++)
			{
				action(copy.get_value(), index);
			}
		}

		/// <summary>
		/// Determines the lowest value of
		/// the range
		/// </summary>
		_NODISCARD value_type min() const
		{
			range_type copy = this->range;
			
			if (!copy.move_next())
				throw sequence_empty_exception();

			auto record = copy.get_value();

			while (copy.move_next())
			{
				const auto value = copy.get_value();

				if (value < record)
					record = value;
			}

			return record;
		}

		/// <summary>
		/// Determines the lowest value of
		/// the range
		/// </summary>
		/// <param name="transformation">a transformation function for each element</param>
		template<typename TLambda, typename TResult = std::invoke_result_t<TLambda, value_type>>
		_NODISCARD TResult min(const TLambda & transformation) const
		{
			range_type copy = this->range;
			
			if (!copy.move_next())
				throw sequence_empty_exception();

			auto record = transformation(copy.get_value());

			while (copy.move_next())
			{
				const auto value = transformation(copy.get_value());

				if (value < record)
					record = value;
			}

			return record;
		}

		/// <summary>
		/// Determines the highest value of
		/// the range
		/// </summary>
		_NODISCARD value_type max() const
		{
			range_type copy = this->range;

			if (!copy.move_next())
				throw sequence_empty_exception();

			auto record = copy.get_value();

			while (copy.move_next())
			{
				const auto value = copy.get_value();

				if (value > record)
					record = value;
			}

			return record;
		}

		/// <summary>
		/// Determines the highest value of
		/// the range
		/// </summary>
		/// <param name="transformation">a transformation function for each element</param>
		template<typename TLambda, typename TResult = std::invoke_result_t<TLambda, value_type>>
		_NODISCARD TResult max(const TLambda & transformation) const
		{
			range_type copy = this->range;

			if (!copy.move_next())
				throw sequence_empty_exception();

			auto record = transformation(copy.get_value());

			while (copy.move_next())
			{
				const auto value = transformation(copy.get_value());

				if (value > record)
					record = value;
			}

			return record;
		}

		/// <summary>
		/// Determines the number of elements held by the range
		/// </summary>
		/// <returns>the number of elements</returns>
		_NODISCARD size_t count() const
		{
			range_type copy = this->range;
			
			size_t count = 0;

			while (copy.move_next())
				++count;

			return count;
		}

		/// <summary>
		/// Determines the number of elements held by the range satisfying
		/// the condition of the predicate
		/// </summary>
		/// <param name="predicate">a predicate for each element deciding whether to count that element or not</param>
		/// <returns>the number of elements satisfying the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		_NODISCARD size_t count(const TPredicate & predicate) const
		{
			range_type copy = this->range;
			
			size_t count = 0;

			while (copy.move_next())
				if (predicate(copy.get_value()))
					++count;

			return count;
		}

		/// <summary>
		/// Determines whether the range has an element or
		/// is empty
		/// </summary>
		/// <returns>True if the range has at least one element to process</returns>
		_NODISCARD bool any() const
		{
			range_type copy = this->range;

			return copy.move_next();
		}

		/// <summary>
		/// Iterates through the range and checks the predicate on each value of
		/// the range
		/// </summary>
		/// <param name="predicate">the predicate which needs to be satisfied by each range-value</param>
		/// <returns></returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		_NODISCARD bool any(const TPredicate & predicate) const
		{
			range_type copy = this->range;

			while (copy.move_next())
			{
				if (predicate(copy.get_value()))
					return true;
			}

			return false;
		}

		/// <summary>
		/// Iterates through the range and checks the predicate on each value of
		/// the range
		/// </summary>
		/// <param name="predicate">the predicate which needs to be satisfied by each range-value</param>
		/// <returns></returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		_NODISCARD bool all(const TPredicate & predicate) const
		{
			range_type copy = this->range;

			while (copy.move_next())
			{
				if (!predicate(copy.get_value()))
					return false;
			}

			return true;
		}

		/// <summary>
		/// searches for a value by iterating through the range and
		/// comparing each value with the given one using the == operator
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>True if the value was found</returns>
		_NODISCARD bool contains(const value_type & value) const
		{
			range_type copy = this->range;
			
			while (copy.move_next())
			{
				if (copy.get_value() == value)
					return true;
			}

			return false;
		}

		/// <summary>
		/// iterates through the range and tries to satisfy the predicate
		/// </summary>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>True if the predicate returned once</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		_NODISCARD bool contains_if(const TPredicate & predicate) const
		{
			range_type copy = this->range;
			
			while (copy.move_next())
			{
				if (predicate(copy.get_value()))
					return true;
			}

			return false;
		}

		/// <summary>
		/// Determines the average of the range
		/// </summary>
		/// <returns>the average of the range</returns>
		_NODISCARD value_type avg() const
		{
			range_type copy = this->range;
			
			// we don't have any elements to work with
			if (!copy.move_next())
				throw sequence_empty_exception();

			auto value = copy.get_value();
			size_t count = 1;

			while (copy.move_next())
			{
				value += copy.get_value();
				++count;
			}

			return value / count;
		}

		/// <summary>
		/// Determines the average of the range
		/// </summary>
		/// <param name="transformation">a function to transform the values of the range</param>
		/// <returns>the average of the range</returns>
		template<typename TTransformation, typename TResult = std::invoke_result_t<TTransformation, value_type>>
		_NODISCARD TResult avg(const TTransformation & transformation) const
		{
			range_type copy = this->range;
			
			// we don't have any elements to work with
			if (!copy.move_next())
				throw sequence_empty_exception();

			auto value = transformation(copy.get_value());
			size_t count = 1;

			while (copy.move_next())
			{
				value += transformation(copy.get_value());
				++count;
			}

			return value / count;
		}

		/// <summary>
		/// Determines the sum of the range
		/// </summary>
		/// <returns>the sum of the range</returns>
		_NODISCARD value_type sum() const
		{
			range_type copy = this->range;
			
			// we don't have any elements to work with
			if (!copy.move_next())
				throw sequence_empty_exception();

			auto value = copy.get_value();

			while (copy.move_next())
			{
				value += copy.get_value();
			}

			return value;
		}

		/// <summary>
		/// Determines the sum of the range
		/// </summary>
		/// <param name="transformation">a function to transform the values of the range</param>
		/// <returns>the sum of the range</returns>
		template<typename TTransformation, typename TResult = std::invoke_result_t<TTransformation, value_type>>
		_NODISCARD TResult sum(const TTransformation & transformation) const
		{
			range_type copy = this->range;
			
			// we don't have any elements to work with
			if (!copy.move_next())
				throw sequence_empty_exception();

			auto value = transformation(copy.get_value());

			while (copy.move_next())
			{
				value += transformation(copy.get_value());
			}

			return value;
		}

		/// <summary>
		/// aggregates each value with an accumulator starting at a certain seed
		/// </summary>
		/// <typeparam name="TAccumulate">the starting-value for the aggregation</typeparam>
		/// <typeparam name="TAccumulator">the accumulator to use for each value in the list</typeparam>
		/// <param name="seed">the starting value of the aggregation</param>
		/// <param name="accumulator">a function to accumulate a value for each element in the list</param>
		template<typename TAccumulate, typename TAccumulator, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TAccumulator, value_type, value_type>, TAccumulate>>>
		_NODISCARD TAccumulate aggregate(const TAccumulate & seed, const TAccumulator & accumulator) const
		{
			range_type copy = this->range;
			
			auto value = seed;

			while (copy.move_next())
			{
				value = accumulator(value, copy.get_value());
			}

			return value;
		}

		/// <summary>
		/// aggregates each value with an accumulator starting at a certain seed
		/// and returns the transformed result
		/// </summary>
		/// <typeparam name="TAccumulate">the starting-value for the aggregation</typeparam>
		/// <typeparam name="TAccumulator">the accumulator to use for each value in the list</typeparam>
		/// <typeparam name="TTransformation">the transformation type used for the final result</typeparam>
		/// <typeparam name="TResult">result-type of the transformation</typeparam>
		/// <param name="seed">the starting value of the aggregation</param>
		/// <param name="accumulator">a function to accumulate a value for each element in the list</param>
		/// <param name="transformation">the transformation for the result</param>
		template<typename TAccumulate, typename TAccumulator, typename TTransformation, typename TResult = std::invoke_result_t<TTransformation, TAccumulate>, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TAccumulator, value_type, value_type>, TAccumulate>>>
		_NODISCARD TResult aggregate(const TAccumulate & seed, const TAccumulator & accumulator, const TTransformation & transformation) const
		{
			return transformation(this->aggregate(seed, accumulator));
		}

		/// <summary>
		/// Looks up a specific element at a certain index
		/// in the range.
		///
		/// This method throws
		/// an index_out_of_bounds_exception in case
		/// the index is too low or too high
		/// </summary>
		/// <param name="index">the index of the element</param>
		/// <returns>the value stored behind that index</returns>
		_NODISCARD value_type element_at(const size_t index) const
		{
			range_type copy = this->range;
			
			size_t current = 0;
			while (current <= index)
			{
				if (!copy.move_next())
				{
					throw index_out_of_bounds_exception();
				}

				current++;
			}

			return copy.get_value();
		}

		/// <summary>
		/// Looks up a specific element at a certain index
		/// in the range.
		///
		/// If the index is out of bounds, the default is returned.
		/// 
		/// </summary>
		/// <param name="index">the index of the element</param>
		/// <returns>the value stored behind that index</returns>
		_NODISCARD value_type element_at_default(const size_t index) const
		{
			range_type copy = this->range;
			
			size_t current = 0;
			while (current <= index)
			{
				if (!copy.move_next())
				{
					return value_type{};
				}

				current++;
			}

			return copy.get_value();
		}

		/// <summary>
		/// Determines the first element of
		/// the range.
		/// If the range is empty, a sequence_empty_exception will
		/// be thrown
		/// </summary>
		_NODISCARD value_type first() const
		{
			range_type copy = this->range;
			
			if (copy.move_next())
				return copy.get_value();

			throw sequence_empty_exception();
		}

		/// <summary>
		/// Determines the first element of the range.
		/// If the range is empty, the default will be returned
		/// </summary>
		template<typename = std::enable_if_t<std::is_default_constructible_v<value_type>>>
		_NODISCARD value_type first_or_default() const
		{
			range_type copy = this->range;
			
			if (copy.move_next())
				return copy.get_value();

			return value_type{};
		}

		/// <summary>
		/// Determines the first element of the range which satisfies the predicate.
		/// If the range is empty, a sequence_empty_exception will be thrown.
		/// 
		/// </summary>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>the first element in the range which satisfies the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		_NODISCARD return_type first(const TPredicate & predicate) const
		{
			range_type copy = this->range;
			
			while (copy.move_next())
			{
				const auto value = copy.get_value();

				if (predicate(value))
					return value;
			}

			throw sequence_empty_exception();
		}

		/// <summary>
		/// Determines the first element of the range which satisfies the predicate.
		/// If the range is empty, the default will be returned.
		/// 
		/// </summary>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>the first element in the range which satisfies the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate> && std::is_default_constructible_v<value_type>>>
		_NODISCARD value_type first_or_default(const TPredicate & predicate) const
		{
			range_type copy = this->range;
			
			while (copy.move_next())
			{
				const auto value = copy.get_value();

				if (predicate(value))
					return value;
			}

			return value_type{};
		}
		
		/// <summary>
		/// Determines the last element of
		/// the range.
		/// If the range is empty, a sequence_empty_exception will
		/// be thrown
		/// </summary>
		_NODISCARD return_type last() const
		{
			range_type copy = this->range;
			
			if (!copy.move_next())
				throw sequence_empty_exception();

			auto result = copy.get_value();

			while (copy.move_next())
			{
				result = copy.get_value();
			}

			return result;
		}

		/// <summary>
		/// Determines the last element of the range.
		/// If the range is empty, the default will be returned
		/// </summary>
		template<typename = std::enable_if_t<std::is_default_constructible_v<value_type>>>
		_NODISCARD value_type last_or_default() const
		{
			range_type copy = this->range;
			
			if (!copy.move_next())
				return value_type{};

			auto result = copy.get_value();

			while (copy.move_next())
			{
				result = copy.get_value();
			}

			return result;
		}

		/// <summary>
		/// Determines the last element of the range which satisfies the predicate.
		/// If the range is empty, a sequence_empty_exception will be thrown.
		/// 
		/// </summary>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>the last element in the range which satisfies the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		_NODISCARD return_type last(const TPredicate & predicate) const
		{
			range_type copy = this->range;
			
			if (!copy.move_next())
				throw sequence_empty_exception();

			auto result = copy.get_value();
			bool updated = false;

			while (copy.move_next())
			{
				const auto value = copy.get_value();

				if (predicate(value))
				{
					result = value;
					updated = true;
				}
			}

			if (updated)
				return result;

			if (predicate(result))
				return result;

			throw sequence_empty_exception();
		}

		/// <summary>
		/// Determines the last element of the range which satisfies the predicate.
		/// If the range is empty, the default will be returned.
		/// 
		/// </summary>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>the last element in the range which satisfies the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate> && std::is_default_constructible_v<value_type>>>
		_NODISCARD value_type last_or_default(const TPredicate & predicate) const
		{
			range_type copy = this->range;
			
			if (!copy.move_next())
				return value_type{};

			auto result = copy.get_value();
			bool updated = false;

			while (copy.move_next())
			{
				const auto value = copy.get_value();

				if (predicate(value))
				{
					result = value;
					updated = true;
				}
			}

			if (updated)
				return result;

			if (predicate(result))
				return result;

			return value_type{};
		}

		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		_NODISCARD enumerable<orderby_range<range_type, TSelector>> orderby(const TSelector & selector) const
		{
			return orderby_ascending(selector);
		}

		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		_NODISCARD enumerable<orderby_range<range_type, TSelector>> orderby_ascending(const TSelector & selector) const
		{
			return enumerable<orderby_range<range_type, TSelector>>(
				orderby_range<range_type, TSelector>(this->range, selector, true)
			);
		}

		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		_NODISCARD enumerable<orderby_range<range_type, TSelector>> orderby_descending(const TSelector & selector) const
		{
			return enumerable<orderby_range<range_type, TSelector>>(
				orderby_range<range_type, TSelector>(this->range, selector, false)
			);
		}

		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		_NODISCARD enumerable<thenby_range<range_type, TSelector>> thenby(const TSelector & selector) const
		{
			return thenby_ascending(selector);
		}


		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		_NODISCARD enumerable<thenby_range<range_type, TSelector>> thenby_ascending(const TSelector & selector) const
		{
			return enumerable<thenby_range<range_type, TSelector>>(
				thenby_range<range_type, TSelector>(this->range, selector, true)
			);
		}

		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		_NODISCARD enumerable<thenby_range<range_type, TSelector>> thenby_descending(const TSelector & selector) const
		{
			return enumerable<thenby_range<range_type, TSelector>>(
				thenby_range<range_type, TSelector>(this->range, selector, false)
			);
		}

		template<range_concept TOtherRange>
		_NODISCARD bool sequence_equal(const enumerable<TOtherRange> & enumerable) const
		{
			range_type lhs  = this->range;
			TOtherRange rhs = enumerable.to_range();
			
			while (true)
			{
				const bool lhs_move = lhs.move_next();
				const bool rhs_move = rhs.move_next();

				if (lhs_move != rhs_move)
				{
					return false;
				}

				if (!lhs_move && !rhs_move)
				{
					return true;
				}

				if (lhs.get_value() != rhs.get_value())
				{
					return false;
				}
			}
		}

		template<range_concept TOtherRange, typename TPredicate, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TPredicate, value_type, value_type>, bool>>>
		_NODISCARD bool sequence_equal(const enumerable<TOtherRange> & enumerable, const TPredicate & predicate) const
		{
			range_type lhs = this->range;
			TOtherRange rhs = enumerable.to_range();

			while (true)
			{
				const bool lhs_move = lhs.move_next();
				const bool rhs_move = rhs.move_next();

				if (lhs_move != rhs_move)
				{
					return false;
				}

				if (!lhs_move && !rhs_move)
				{
					return true;
				}

				if (!predicate(lhs.get_value(), rhs.get_value()))
				{
					return false;
				}
			}
		}

		template<typename TEnumerableSelection, typename = std::enable_if_t<std::is_invocable_v<TEnumerableSelection, typename range_type::value_type>>>
		_NODISCARD enumerable<select_many_range<range_type, TEnumerableSelection>> select_many(const TEnumerableSelection & selection) const
		{
			return enumerable<select_many_range<range_type, TEnumerableSelection>>(
				select_many_range<range_type, TEnumerableSelection>(this->range, selection)
			);
		}

		_NODISCARD enumerable<pairwise_range<range_type>> pairwise() const
		{
			return enumerable<pairwise_range<range_type>>(
				pairwise_range<range_type>(this->range)
			);
		}
		
		_NODISCARD std::basic_string<char> concatenate(
			const std::string & separator,
			size_t capacity = 16
		)
		{
			return concatenate_impl<char>(this->range, separator, capacity);
		}

		_NODISCARD std::basic_string<wchar_t> concatenate(
			const std::wstring & separator,
			size_t capacity = 16
		)
		{
			return concatenate_impl<wchar_t>(this->range, separator, capacity);
		}

		template<
			enumerable_concept TEnumerable,
			typename TLhsIdSelection,
			typename TRhsIdSelection,
			typename TJoinSelection,
			typename = std::enable_if_t<
				std::is_invocable_v<TLhsIdSelection, value_type> &&
				std::is_invocable_v<TRhsIdSelection, typename TEnumerable::value_type> &&
				std::is_invocable_v<TJoinSelection, typename range_type::value_type, typename TEnumerable::value_type>
			>
		>
		_NODISCARD enumerable<join_range<range_type, TEnumerable, TLhsIdSelection, TRhsIdSelection, TJoinSelection>> join(
			const TEnumerable & enumerable_value,
			const TLhsIdSelection & lhs_id_selection,
			const TRhsIdSelection & rhs_id_selection,
			const TJoinSelection & join_selection
		) const
		{
			return enumerable<join_range<range_type, TEnumerable, TLhsIdSelection, TRhsIdSelection, TJoinSelection>>(
				join_range<range_type, TEnumerable, TLhsIdSelection, TRhsIdSelection, TJoinSelection>(
					this->range,
					enumerable_value.to_range(),
					lhs_id_selection,
					rhs_id_selection,
					join_selection
				)
			);
		}

		template<template<typename, typename> typename TMap = std::map, typename TKeySelection, typename = std::enable_if_t<std::is_invocable_v<TKeySelection, value_type>>>
		_NODISCARD TMap<std::invoke_result_t<TKeySelection, value_type>, value_type> to_map(const TKeySelection & key_selection) const
		{
			range_type copy = this->range;
			
			TMap<std::invoke_result_t<TKeySelection, value_type>, value_type> result;
			while (copy.move_next())
			{
				const auto value = copy.get_value();
				const auto key = key_selection(value);
				result.insert({ key, value });
			}
			return result;
		}

		template<template<typename> typename TSet = std::set>
		_NODISCARD TSet<value_type> to_set() const
		{
			range_type copy = this->range;
			
			TSet<value_type> result;

			while (copy.move_next())
			{
				result.insert(copy.get_value());
			}

			return result;
		}

		template<template<typename> typename TQueue = std::queue>
		_NODISCARD TQueue<value_type> to_queue() const
		{
			range_type copy = this->range;
			
			TQueue<value_type> result;

			while (copy.move_next())
			{
				result.push(copy.get_value());
			}

			return result;
		}

		_NODISCARD std::stack<value_type> to_stack() const
		{
			range_type copy = this->range;
			
			std::stack<value_type> result;

			while (copy.move_next())
			{
				result.push(copy.get_value());
			}

			return result;
		}
		
		template<typename TEnumerable>
		_NODISCARD enumerable<union_range<range_type, TEnumerable>> union_with(const TEnumerable & enumerable_range) const
		{
			return enumerable<union_range<range_type, TEnumerable>>(
				union_range<range_type, TEnumerable>(
					this->range,
					enumerable_range.to_range()
				)
			);
		}

		_NODISCARD enumerable<shuffle_range<range_type>> shuffle() const
		{
			return enumerable<shuffle_range<range_type>>(
				shuffle_range<range_type>(this->range)
			);
		}

		template<typename TKeySelector>
		_NODISCARD lookup<std::invoke_result_t<TKeySelector, value_type>, value_type> to_lookup(const TKeySelector & selector) const
		{
			using key_type = std::invoke_result_t<TKeySelector, value_type>;
			
			return lookup<key_type, value_type>(this->range, selector);
		}

		template<typename TValue, typename = std::enable_if_t<std::is_convertible_v<value_type, TValue>>>
		_NODISCARD auto cast() const
		{
			return this->select([](const value_type & value)
			{
				return static_cast<TValue>(value);
			});
		}
		
		_NODISCARD value_type single() const
		{
			range_type copy = this->range;
			
			if (!copy.move_next())
				throw invalid_operation_exception();

			return copy.get_value();
		}

		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		_NODISCARD value_type single(const TPredicate & predicate) const
		{
			range_type copy = this->range;
			
			while (copy.move_next())
			{
				const return_type value = copy.get_value();

				if (predicate(value))
					return value;
			}

			throw invalid_operation_exception();
		}

		template<enumerable_concept TEnumerable>
		_NODISCARD enumerable<zip_with_range<range_type, typename TEnumerable::range_type>> zip_with(const TEnumerable & other) const
		{
			return enumerable<zip_with_range<range_type, typename TEnumerable::range_type>>(
				zip_with_range<range_type, typename TEnumerable::range_type>(this->range, other.to_range())
			);
		}
		
	private:

		template<typename TChar>
		static _NODISCARD std::basic_string<TChar> concatenate_impl(
			range_type range,
			const std::basic_string<TChar> & separator,
			size_t capacity
		)
		{
			std::vector<TChar> buffer;
			buffer.reserve(capacity);
			bool first = true;
			
			while (range.move_next())
			{
				if(first)
				{
					first = false;
				} else
				{
					buffer.insert(buffer.end(), separator.begin(), separator.end());
				}

				const auto current_value = range.get_value();
				buffer.insert(buffer.end(), current_value.begin(), current_value.end());
			}

			return std::basic_string<TChar>(buffer.begin(), buffer.end());
		}
		
	private:
		
		/// <summary>
		/// Member attributes
		/// </summary>

		range_type range;
		
	};
	
	/// <summary>
	/// Creates an enumerable from
	/// an array
	/// </summary>
	/// <typeparam name="TArray">type of an array element</typeparam>
	/// <param name="array">array instance</param>
	/// <returns>an enumerable of type iterator_range</returns>
	template<typename TArray, int ArraySize>
	_NODISCARD enumerable<iterator_range<typename array_traits<TArray[ArraySize]>::iterator>> from(const TArray(& array)[ArraySize])
	{
		using iterator = typename array_traits<TArray[ArraySize]>::iterator;
		const iterator begin = array;
		const iterator end   = array + ArraySize;
		
		return enumerable<iterator_range<iterator>>(
			iterator_range<iterator>(begin, end)
		);
	}

	/// <summary>
	/// Creates an enumerable from
	/// a container object
	/// </summary>
	/// <typeparam name="TContainer">a container type which has the begin and end method</typeparam>
	/// <param name="container">the container holding the values to store</param>
	/// <returns>An enumerable object holding the values provided from the container</returns>
	template<container_concept TContainer>
	_NODISCARD enumerable<iterator_range<typename TContainer::const_iterator>> from(
		const TContainer & container
	)
	{
		return enumerable<iterator_range<typename TContainer::const_iterator>>(
			iterator_range<typename TContainer::const_iterator>(
				container.begin(),
				container.end()
			)
		);
	}

	/// <summary>
	/// Constructs an enumerable object from a raw iterator
	/// </summary>
	/// <typeparam name="TIterator">the iterator-type</typeparam>
	/// <param name="begin">begin of the iterator</param>
	/// <param name="end">end of the iterator</param>
	/// <returns>enumerable object holding the iterator as range</returns>
	template<typename TIterator>
	_NODISCARD enumerable<iterator_range<TIterator>> from(
		const TIterator & begin,
		const TIterator & end
	)
	{
		return enumerable<iterator_range<TIterator>>(
			iterator_range<TIterator>(begin, end)
		);
	}

	/// <summary>
	/// Creates a repeating range for n times
	/// </summary>
	/// <typeparam name="TValue">the value-type of the specific value to repeat</typeparam>
	/// <param name="value">the actual value to repeat</param>
	/// <param name="repetitions">the number of times to repeat the value</param>
	template<typename TValue>
	_NODISCARD enumerable<repeat_range<TValue>> repeat(const TValue & value, size_t repetitions)
	{
		return enumerable<repeat_range<TValue>>(
			repeat_range<TValue>(value, repetitions)
		);
	}

	/// <summary>
	/// Creates an empty range
	/// </summary>
	/// <typeparam name="TValue">the value-type of the non-existing value</typeparam>
	template<typename TValue>
	_NODISCARD enumerable<empty_range<TValue>> empty()
	{
		return enumerable<empty_range<TValue>>(
			empty_range<TValue>()
		);
	}

	/// <summary>
	/// Creates an incrementing range
	/// </summary>
	/// <typeparam name="TValue">the value-type of the start, end and incrementing values</typeparam>
	/// <param name="start">inclusive starting value</param>
	/// <param name="end">inclusive end value</param>
	/// <param name="increment">the amount to increment in each iteration</param>
	template<typename TValue>
	_NODISCARD enumerable<increment_range<TValue>> range(
		const TValue & start,
		const TValue & end,
		const TValue & increment
	)
	{
		return enumerable<increment_range<TValue>>(
			increment_range<TValue>(start, end, increment)
		);
	}
	
}

#include <linq/ranges/lookup.hpp>