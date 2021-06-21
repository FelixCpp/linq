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
#include <linq/ranges/intersects_range.hpp>
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

#include <linq/utils/array_traits.hpp>
#include <linq/utils/concepts.hpp>

namespace linq
{

	template<range_concept TRange, typename TGroupSelector>
	class lookup_table;

	template<range_concept TRange>
	class enumerable
	{
	public:
		
		/// <summary>
		/// type definitions
		/// </summary>
		using range_type = TRange;
		using value_type = typename TRange::value_type;

	private:

		template<typename TLambda, typename TResult>    inline static constexpr bool is_lambda    = std::is_same_v<std::invoke_result_t<TLambda, value_type>, TResult>;
		template<typename TPredicate>                   inline static constexpr bool is_predicate = is_lambda<TPredicate, bool>;
		template<typename TAction>                      inline static constexpr bool is_action    = is_lambda<TAction, void>;
		
	public:

		/// <summary>
		/// Constructs an enumerable with
		/// a given range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		constexpr explicit enumerable(const range_type & range)
			: range(range)
		{
		}

		/// <summary>
		/// Returns the range of the enumerable
		/// </summary>
		const range_type & to_range() const
		{
			return this->range;
		}
		
		/// <summary>
		/// Creates a new enumerable which ignores the values
		/// stored in the except_range
		/// </summary>
		/// <param name="collection">an enumerable holding each value to ignore</param>
		template<range_concept TExceptRange>
		constexpr auto except(const enumerable<TExceptRange> & collection) const
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
		constexpr auto concat(const enumerable<TConcatRange> & collection) const
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
		constexpr auto where(const TPredicate & predicate) const
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
		constexpr enumerable<select_range<range_type, TLambda>> select(const TLambda & transformation) const
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
		constexpr enumerable<intersects_range<range_type, TIntersectsRange>> intersects(const enumerable<TIntersectsRange> & collection) const
		{
			return enumerable<intersects_range<range_type, TIntersectsRange>>(
				intersects_range<range_type, TIntersectsRange>(this->range, collection.to_range())
			);
		}

		/// <summary>
		/// Removes all duplicates from the range
		/// </summary>
		constexpr enumerable<distinct_range<range_type>> distinct() const
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
		constexpr enumerable<skip_range<range_type>> skip(size_t count) const
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
		constexpr enumerable<take_range<range_type>> take(size_t count) const
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
		constexpr enumerable<skip_while_range<range_type, TPredicate>> skip_while(const TPredicate & predicate) const
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
		constexpr enumerable<take_while_range<range_type, TPredicate>> take_while(const TPredicate & predicate) const
		{
			return enumerable<take_while_range<range_type, TPredicate>>(
				take_while_range<range_type, TPredicate>(this->range, predicate)
			);
		}

		/// <summary>
		/// Reverses the range
		/// </summary>
		constexpr enumerable<reverse_range<range_type>> reverse()
		{
			return enumerable<reverse_range<range_type>>(
				reverse_range<range_type>(this->range)
			);
		}

		/// <summary>
		/// creates a list based on the values of the range
		/// </summary>
		constexpr std::list<value_type> to_list() const
		{
			return to_list_impl(this->range);
		}

		/// <summary>
		/// creates a vector based on the values of the range
		/// </summary>
		constexpr std::vector<value_type> to_vector(size_t capacity = 16) const
		{
			return to_vector_impl(this->range, capacity);
		}

		/// <summary>
		/// Iterates through the range and performs an action
		/// for each value
		/// </summary>
		/// <param name="action">the action to perform for each value</param>
		template<typename TAction, typename = std::enable_if_t<is_action<TAction>>>
		constexpr void for_each(const TAction & action) const
		{
			for_each_impl(this->range, action);
		}

		/// <summary>
		/// Iterates through the range and performs an action
		/// for each value
		/// </summary>
		/// <param name="action">the action to perform for each value</param>
		template<typename TAction, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TAction, value_type, size_t>, void>>>
		constexpr void indexed_for_each(const TAction & action) const
		{
			indexed_for_each_impl(this->range, action);
		}

		/// <summary>
		/// Determines the lowest value of
		/// the range
		/// </summary>
		constexpr value_type min() const
		{
			return min_impl(this->range);
		}

		/// <summary>
		/// Determines the lowest value of
		/// the range
		/// </summary>
		/// <param name="transformation">a transformation function for each element</param>
		template<typename TLambda, typename TResult = std::invoke_result_t<TLambda, value_type>>
		constexpr TResult min(const TLambda & transformation) const
		{
			return min_impl(this->range, transformation);
		}

		/// <summary>
		/// Determines the highest value of
		/// the range
		/// </summary>
		constexpr value_type max() const
		{
			return max_impl(this->range);
		}

		/// <summary>
		/// Determines the highest value of
		/// the range
		/// </summary>
		/// <param name="transformation">a transformation function for each element</param>
		template<typename TLambda, typename TResult = std::invoke_result_t<TLambda, value_type>>
		constexpr TResult max(const TLambda & transformation) const
		{
			return max_impl(this->range, transformation);
		}

		/// <summary>
		/// Determines the number of elements held by the range
		/// </summary>
		/// <returns>the number of elements</returns>
		constexpr size_t count() const
		{
			return count_impl(this->range);
		}

		/// <summary>
		/// Determines the number of elements held by the range satisfying
		/// the condition of the predicate
		/// </summary>
		/// <param name="predicate">a predicate for each element deciding whether to count that element or not</param>
		/// <returns>the number of elements satisfying the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr size_t count_if(const TPredicate & predicate) const
		{
			return count_if_impl(this->range, predicate);
		}

		/// <summary>
		/// Determines whether the range has an element or
		/// is empty
		/// </summary>
		/// <returns>True if the range has at least one element to process</returns>
		constexpr bool any() const
		{
			return any_impl(this->range);
		}

		/// <summary>
		/// Iterates through the range and checks the predicate on each value of
		/// the range
		/// </summary>
		/// <param name="predicate">the predicate which needs to be satisfied by each range-value</param>
		/// <returns></returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr bool any(const TPredicate & predicate) const
		{
			return any_if_impl(this->range, predicate);
		}

		/// <summary>
		/// Iterates through the range and checks the predicate on each value of
		/// the range
		/// </summary>
		/// <param name="predicate">the predicate which needs to be satisfied by each range-value</param>
		/// <returns></returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr bool all(const TPredicate & predicate) const
		{
			return all_impl(this->range, predicate);
		}

		/// <summary>
		/// searches for a value by iterating through the range and
		/// comparing each value with the given one using the == operator
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>True if the value was found</returns>
		constexpr bool contains(const value_type & value) const
		{
			return contains_impl(this->range, value);
		}

		/// <summary>
		/// iterates through the range and tries to satisfy the predicate
		/// </summary>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>True if the predicate returned once</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr bool contains_if(const TPredicate & predicate) const
		{
			return contains_if_impl(this->range, predicate);
		}

		/// <summary>
		/// Determines the average of the range
		/// </summary>
		/// <returns>the average of the range</returns>
		constexpr value_type avg() const
		{
			return avg_impl(this->range);
		}

		/// <summary>
		/// Determines the average of the range
		/// </summary>
		/// <param name="transformation">a function to transform the values of the range</param>
		/// <returns>the average of the range</returns>
		template<typename TTransformation, typename TResult = std::invoke_result_t<TTransformation, value_type>>
		constexpr TResult avg(const TTransformation & transformation) const
		{
			return avg_impl(this->range, transformation);
		}

		/// <summary>
		/// Determines the sum of the range
		/// </summary>
		/// <returns>the sum of the range</returns>
		constexpr value_type sum() const
		{
			return sum_impl(this->range);
		}

		/// <summary>
		/// Determines the sum of the range
		/// </summary>
		/// <param name="transformation">a function to transform the values of the range</param>
		/// <returns>the sum of the range</returns>
		template<typename TTransformation, typename TResult = std::invoke_result_t<TTransformation, value_type>>
		constexpr TResult sum(const TTransformation & transformation) const
		{
			return sum_impl(this->range, transformation);
		}

		/// <summary>
		/// aggregates each value with an accumulator starting at a certain seed
		/// </summary>
		/// <typeparam name="TAccumulate">the starting-value for the aggregation</typeparam>
		/// <typeparam name="TAccumulator">the accumulator to use for each value in the list</typeparam>
		/// <param name="seed">the starting value of the aggregation</param>
		/// <param name="accumulator">a function to accumulate a value for each element in the list</param>
		template<typename TAccumulate, typename TAccumulator, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TAccumulator, value_type, value_type>, TAccumulate>>>
		constexpr TAccumulate aggregate(const TAccumulate & seed, const TAccumulator & accumulator) const
		{
			return aggregate_impl(this->range, seed, accumulator);
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
		constexpr TResult aggregate(const TAccumulate & seed, const TAccumulator & accumulator, const TTransformation & transformation) const
		{
			return aggregate_impl(this->range, seed, accumulator, transformation);
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
		constexpr value_type element_at(const size_t index) const
		{
			return element_at_impl(this->range, index);
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
		constexpr value_type element_at_default(const size_t index) const
		{
			return element_at_or_default_impl(this->range, index);
		}

		/// <summary>
		/// Determines the first element of
		/// the range.
		/// If the range is empty, a sequence_empty_exception will
		/// be thrown
		/// </summary>
		constexpr value_type first() const
		{
			return first_impl(this->range);
		}

		/// <summary>
		/// Determines the first element of the range.
		/// If the range is empty, the default will be returned
		/// </summary>
		constexpr value_type first_or_default() const
		{
			return first_or_default_impl(this->range);
		}

		/// <summary>
		/// Determines the first element of the range which satisfies the predicate.
		/// If the range is empty, a sequence_empty_exception will be thrown.
		/// 
		/// </summary>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>the first element in the range which satisfies the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr value_type first(const TPredicate & predicate) const
		{
			return first_impl(this->range, predicate);
		}

		/// <summary>
		/// Determines the first element of the range which satisfies the predicate.
		/// If the range is empty, the default will be returned.
		/// 
		/// </summary>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>the first element in the range which satisfies the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr value_type first_or_default(const TPredicate & predicate) const
		{
			return first_or_default_impl(this->range, predicate);
		}
		
		/// <summary>
		/// Determines the last element of
		/// the range.
		/// If the range is empty, a sequence_empty_exception will
		/// be thrown
		/// </summary>
		constexpr value_type last() const
		{
			return last_impl(this->range);
		}

		/// <summary>
		/// Determines the last element of the range.
		/// If the range is empty, the default will be returned
		/// </summary>
		constexpr value_type last_or_default() const
		{
			return last_or_default_impl(this->range);
		}

		/// <summary>
		/// Determines the last element of the range which satisfies the predicate.
		/// If the range is empty, a sequence_empty_exception will be thrown.
		/// 
		/// </summary>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>the last element in the range which satisfies the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr value_type last(const TPredicate & predicate) const
		{
			return last_impl(this->range, predicate);
		}

		/// <summary>
		/// Determines the last element of the range which satisfies the predicate.
		/// If the range is empty, the default will be returned.
		/// 
		/// </summary>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>the last element in the range which satisfies the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr value_type last_or_default(const TPredicate & predicate) const
		{
			return last_or_default_impl(this->range, predicate);
		}

		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		constexpr enumerable<orderby_range<range_type, TSelector>> orderby(const TSelector & selector) const
		{
			return orderby_ascending(selector);
		}


		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		constexpr enumerable<orderby_range<range_type, TSelector>> orderby_ascending(const TSelector & selector) const
		{
			return enumerable<orderby_range<range_type, TSelector>>(
				orderby_range<range_type, TSelector>(this->range, selector, true)
			);
		}

		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		constexpr enumerable<orderby_range<range_type, TSelector>> orderby_descending(const TSelector & selector) const
		{
			return enumerable<orderby_range<range_type, TSelector>>(
				orderby_range<range_type, TSelector>(this->range, selector, false)
			);
		}

		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		constexpr enumerable<thenby_range<range_type, TSelector>> thenby(const TSelector & selector) const
		{
			return thenby_ascending(selector);
		}


		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		constexpr enumerable<thenby_range<range_type, TSelector>> thenby_ascending(const TSelector & selector) const
		{
			return enumerable<thenby_range<range_type, TSelector>>(
				thenby_range<range_type, TSelector>(this->range, selector, true)
			);
		}

		template<typename TSelector, typename = std::enable_if_t<std::is_invocable_v<TSelector, value_type>>>
		constexpr enumerable<thenby_range<range_type, TSelector>> thenby_descending(const TSelector & selector) const
		{
			return enumerable<thenby_range<range_type, TSelector>>(
				thenby_range<range_type, TSelector>(this->range, selector, false)
			);
		}

		template<range_concept TOtherRange>
		constexpr bool sequence_equal(const enumerable<TOtherRange> & enumerable) const
		{
			return sequence_equal_impl(this->range, enumerable.to_range());
		}

		template<range_concept TOtherRange, typename TPredicate, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TPredicate, value_type, value_type>, bool>>>
		constexpr bool sequence_equal(const enumerable<TOtherRange> & enumerable, const TPredicate & predicate) const
		{
			return sequence_equal_impl(this->range, enumerable.to_range(), predicate);
		}

		template<typename TEnumerableSelection, typename = std::enable_if_t<std::is_invocable_v<TEnumerableSelection, typename range_type::value_type>>>
		constexpr enumerable<select_many_range<range_type, TEnumerableSelection>> select_many(const TEnumerableSelection & selection) const
		{
			return enumerable<select_many_range<range_type, TEnumerableSelection>>(
				select_many_range<range_type, TEnumerableSelection>(this->range, selection)
			);
		}

		constexpr enumerable<pairwise_range<range_type>> pairwise() const
		{
			return enumerable<pairwise_range<range_type>>(
				pairwise_range<range_type>(this->range)
			);
		}
		
		constexpr std::basic_string<char> concatenate(
			const std::string & separator,
			size_t capacity = 16
		)
		{
			return concatenate_impl<char>(this->range, separator, capacity);
		}

		constexpr std::basic_string<wchar_t> concatenate(
			const std::wstring & separator,
			size_t capacity = 16
		)
		{
			return concatenate_impl<wchar_t>(this->range, separator, capacity);
		}

		template<
			typename TEnumerable,
			typename TLhsIdSelection,
			typename TRhsIdSelection,
			typename TJoinSelection,
			typename = std::enable_if_t<
				std::is_invocable_v<TLhsIdSelection, value_type> &&
				std::is_invocable_v<TRhsIdSelection, typename TEnumerable::range_type::value_type> &&
				std::is_invocable_v<TJoinSelection, typename range_type::value_type, typename TEnumerable::range_type::value_type>
			>
		>
		constexpr enumerable<join_range<range_type, TEnumerable, TLhsIdSelection, TRhsIdSelection, TJoinSelection>> join(
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
		constexpr TMap<std::invoke_result_t<TKeySelection, value_type>, value_type> to_map(const TKeySelection & key_selection) const
		{
			return to_map_impl<TMap, TKeySelection>(this->range, key_selection);
		}

		template<template<typename> typename TSet = std::set>
		constexpr TSet<value_type> to_set() const
		{
			return to_set_impl<TSet>(this->range);
		}

		template<template<typename> typename TQueue = std::queue>
		constexpr TQueue<value_type> to_queue() const
		{
			return to_queue_impl<TQueue>(this->range);
		}

		constexpr std::stack<value_type> to_stack() const
		{
			return to_stack_impl(this->range);
		}
		
		template<typename TEnumerable>
		constexpr enumerable<union_range<range_type, TEnumerable>> union_with(const TEnumerable & enumerable_range) const
		{
			return enumerable<union_range<range_type, TEnumerable>>(
				union_range<range_type, TEnumerable>(
					this->range,
					enumerable_range.to_range()
				)
			);
		}

		constexpr enumerable<shuffle_range<range_type>> shuffle() const
		{
			return enumerable<shuffle_range<range_type>>(
				shuffle_range<range_type>(this->range)
			);
		}

		template<typename TGroupSelector>
		constexpr lookup_table<range_type, TGroupSelector> to_lookup(const TGroupSelector & selector) const
		{
			return lookup_table<range_type, TGroupSelector>(
				lookup_table<range_type, TGroupSelector>(this->range, selector)
			);
		}
	
	private:

		/// <summary>
		/// Implementation to create a list based of a
		/// range
		/// </summary>
		/// <param name="range">the range containing the values to insert into the list</param>
		constexpr static std::list<value_type> to_list_impl(range_type range)
		{
			std::list<value_type> result;
			
			while (range.move_next())
			{
				result.push_back(range.get_value());
			}

			return result;
		}

		/// <summary>
		/// Implementation to create a vector based on the
		/// values of a range
		/// </summary>
		/// <param name="range">the range containing the values</param>
		/// <param name="capacity">the capacity of the vector (performance reasons)</param>
		constexpr static std::vector<value_type> to_vector_impl(range_type range, size_t capacity)
		{
			std::vector<value_type> result;
			result.reserve(capacity);
			
			while (range.move_next())
			{
				result.push_back(range.get_value());
			}

			return result;
		}

		/// <summary>
		/// Implementation for a for-each iteration through the whole range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="action">the action to perform for each value</param>
		template<typename TAction, typename = std::enable_if_t<is_action<TAction>>>
		constexpr static void for_each_impl(range_type range, const TAction & action)
		{
			while (range.move_next())
			{
				action(range.get_value());
			}
		}

		/// <summary>
		/// Implementation for a for-each iteration through the whole range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="action">the action to perform for each value</param>
		template<typename TAction, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TAction, value_type, size_t>, void>>>
		constexpr static void indexed_for_each_impl(range_type range, const TAction & action)
		{
			size_t index = 0;
			while (range.move_next())
			{
				action(range.get_value(), index++);
			}
		}

		/// <summary>
		/// Determines the lowest value of the range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		constexpr static value_type min_impl(range_type range)
		{
			if (!range.move_next())
				throw sequence_empty_exception();

			auto record = range.get_value();

			while (range.move_next())
			{
				const auto value = range.get_value();

				if (value < record)
					record = value;
			}
			
			return record;
		}

		/// <summary>
		/// Determines the lowest value of the range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="transformation">a transformation function for each element</param>
		template<typename TTransformation, typename TResult = std::invoke_result_t<TTransformation, value_type>>
		constexpr static TResult min_impl(range_type range, const TTransformation & transformation)
		{
			if (!range.move_next())
				throw sequence_empty_exception();

			auto record = transformation(range.get_value());

			while (range.move_next())
			{
				const auto value = transformation(range.get_value());

				if (value < record)
					record = value;
			}

			return record;
		}

		/// <summary>
		/// Determines the highest value of the range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		constexpr static value_type max_impl(range_type range)
		{
			if (!range.move_next())
				throw sequence_empty_exception();

			auto record = range.get_value();

			while (range.move_next())
			{
				const auto value = range.get_value();

				if (value > record)
					record = value;
			}

			return record;
		}

		/// <summary>
		/// Determines the highest value of the range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="transformation">a transformation function for each element</param>
		template<typename TTransformation, typename TResult = std::invoke_result_t<TTransformation, value_type>>
		constexpr static TResult max_impl(range_type range, const TTransformation & transformation)
		{
			if (!range.move_next())
				throw sequence_empty_exception();

			auto record = transformation(range.get_value());

			while (range.move_next())
			{
				const auto value = transformation(range.get_value());

				if (value > record)
					record = value;
			}

			return record;
		}

		/// <summary>
		/// Determines the number of elements held by the range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <returns>the number of elements</returns>
		constexpr static size_t count_impl(range_type range)
		{
			size_t count = 0;

			while (range.move_next())
				++count;
			
			return count;
		}

		/// <summary>
		/// Determines the number of elements held by the range satisfying
		/// the condition of the predicate
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="predicate">a predicate for each element deciding whether to count that element or not</param>
		/// <returns>the number of elements satisfying the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr static size_t count_if_impl(range_type range, const TPredicate & predicate)
		{
			size_t count = 0;

			while (range.move_next())
				if(predicate(range.get_value()))
					++count;

			return count;
		}

		/// <summary>
		/// Determines whether the range has an element or
		/// is empty
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <returns>True if the range has at least one element to process</returns>
		constexpr static bool any_impl(range_type range)
		{
			return range.move_next();
		}

		/// <summary>
		/// Iterates through the range and checks the predicate on each value of
		/// the range
		/// </summary>
		/// <param name="range">the range to iterate through</param>
		/// <param name="predicate">the predicate which needs to be satisfied by each range-value</param>
		/// <returns></returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr static bool any_impl(range_type range, const TPredicate & predicate)
		{
			while (range.move_next())
			{
				if (predicate(range.get_value()))
					return true;
			}

			return false;
		}

		/// <summary>
		/// Iterates through the range and checks the predicate on each value of
		/// the range
		/// </summary>
		/// <param name="range">the range to iterate through</param>
		/// <param name="predicate">the predicate which needs to be satisfied by each range-value</param>
		/// <returns></returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr static bool all_impl(range_type range, const TPredicate & predicate)
		{
			while (range.move_next())
			{
				if (!predicate(range.get_value()))
					return false;
			}

			return true;
		}

		/// <summary>
		/// Iterates through the range and compares each value with the given
		/// one by using the == operator
		/// </summary>
		/// <param name="range">the range to iterate through</param>
		/// <param name="value">the value to search for</param>
		/// <returns>True if the value was found</returns>
		constexpr static bool contains_impl(range_type range, const value_type & value)
		{
			while (range.move_next())
			{
				if (range.get_value() == value)
					return true;
			}

			return false;
		}

		/// <summary>
		/// Iterates through the range and compares each value with the given
		/// one by using the == operator
		/// </summary>
		/// <param name="range">the range to iterate through</param>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>True if the value was found</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		constexpr static bool contains_if_impl(range_type range, const TPredicate & predicate)
		{
			while (range.move_next())
			{
				if(predicate(range.get_value()))
					return true;
			}

			return false;
		}

		/// <summary>
		/// Calculates the average value of the range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <returns>the average of the range</returns>
		static constexpr value_type avg_impl(range_type range)
		{
			// we don't have any elements to work with
			if (!range.move_next())
				throw sequence_empty_exception();

			auto value = range.get_value();
			size_t count = 1;

			while (range.move_next())
			{
				value += range.get_value();
				++count;
			}

			return value / count;
		}

		/// <summary>
		/// Calculates the average value of the range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="transformation">a function to transform the values of the range</param>
		/// <returns>the average of the range</returns>
		template<typename TTransformation, typename TResult = std::invoke_result_t<TTransformation, value_type>>
		static constexpr TResult avg_impl(range_type range, const TTransformation & transformation)
		{
			// we don't have any elements to work with
			if (!range.move_next())
				throw sequence_empty_exception();

			auto value = transformation(range.get_value());
			size_t count = 1;

			while (range.move_next())
			{
				value += transformation(range.get_value());
				++count;
			}

			return value / count;
		}

		/// <summary>
		/// Calculates the sum value of the range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <returns>the sum of the range</returns>
		static constexpr value_type sum_impl(range_type range)
		{
			// we don't have any elements to work with
			if (!range.move_next())
				throw sequence_empty_exception();

			auto value = range.get_value();

			while (range.move_next())
			{
				value += range.get_value();
			}

			return value;
		}

		/// <summary>
		/// Calculates the sum value of the range
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="transformation">a function to transform the values of the range</param>
		/// <returns>the sum of the range</returns>
		template<typename TTransformation, typename TResult = std::invoke_result_t<TTransformation, value_type>>
		static constexpr TResult sum_impl(range_type range, const TTransformation & transformation)
		{
			// we don't have any elements to work with
			if (!range.move_next())
				throw sequence_empty_exception();

			auto value = transformation(range.get_value());

			while (range.move_next())
			{
				value += transformation(range.get_value());
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
		/// <param name="range">the range to operate on</param>
		template<typename TAccumulate, typename TAccumulator, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TAccumulator, value_type, value_type>, TAccumulate>>>
		static constexpr TAccumulate aggregate_impl(range_type range, const TAccumulate & seed, const TAccumulator & accumulator)
		{
			auto value = seed;

			while (range.move_next())
			{
				value = accumulator(value, range.get_value());
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
		/// <param name="range">the range to operate on</param>
		/// <param name="seed">the starting value of the aggregation</param>
		/// <param name="accumulator">a function to accumulate a value for each element in the list</param>
		/// <param name="transformation">the transformation for the result</param>
		template<typename TAccumulate, typename TAccumulator, typename TTransformation, typename TResult = std::invoke_result_t<TTransformation, TAccumulate>, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TAccumulator, value_type, value_type>, TAccumulate>>>
		static constexpr TResult aggregate_impl(range_type range, const TAccumulate & seed, const TAccumulator & accumulator, const TTransformation & transformation)
		{
			auto value = seed;
		
			while (range.move_next())
			{
				value = accumulator(value, range.get_value());
			}
		
			return transformation(value);
		}

		/// <summary>
		/// Looks up a specific element at a certain index
		/// in the range.
		///
		/// This method throws
		/// an index_out_of_bounds_exception in case
		/// the index is too low or too high
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="index">the index of the element</param>
		/// <returns>the value stored behind that index</returns>
		static constexpr value_type element_at_impl(range_type range, const size_t index)
		{
			size_t current = 0;
			while (current <= index)
			{
				if(!range.move_next())
				{
					throw index_out_of_bounds_exception();
				}

				current++;
			}

			return range.get_value();
		}

		/// <summary>
		/// Looks up a specific element at a certain index
		/// in the range.
		///
		/// If the index is out of bounds, the default is returned
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="index">the index of the element</param>
		/// <returns>the value stored behind that index</returns>
		static constexpr value_type element_at_or_default_impl(range_type range, const size_t index)
		{
			size_t current = 0;
			while (current <= index)
			{
				if (!range.move_next())
				{
					return value_type{};
				}

				current++;
			}

			return range.get_value();
		}

		/// <summary>
		/// Determines the first element of the range.
		/// If the range is empty, a sequence_empty_exception will be thrown.
		/// 
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <returns>the first element in the list</returns>
		static constexpr value_type first_impl(range_type range)
		{
			if (range.move_next())
				return range.get_value();

			throw sequence_empty_exception();
		}

		/// <summary>
		/// Determines the first element of the range.
		/// If the range is empty, the default will be returned
		/// 
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <returns>the first element in the list</returns>
		static constexpr value_type first_or_default_impl(range_type range)
		{
			if (range.move_next())
				return range.get_value();

			return value_type{};
		}

		/// <summary>
		/// Determines the first element of the range.
		/// If the range is empty, the default will be returned
		/// 
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <returns>the first element in the list</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		static constexpr value_type first_or_default_impl(range_type range, const TPredicate & predicate)
		{
			while (range.move_next())
			{
				const auto value = range.get_value();
				
				if (predicate(value))
					return value;
			}

			return value_type{};
		}
		
		/// <summary>
		/// Determines the first element of the range which satisfies the predicate.
		/// If the range is empty, a sequence_empty_exception will be thrown.
		/// 
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="predicate">the predicate to satisfy</param>
		/// <returns>the first element in the range which satisfies the predicate</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		static constexpr value_type first_impl(range_type range, const TPredicate & predicate)
		{
			while(range.move_next())
			{
				const auto value = range.get_value();

				if (predicate(value))
					return value;
			}

			throw sequence_empty_exception();
		}

		/// <summary>
		/// Determines the first element of the range.
		/// If the range is empty, a sequence_empty_exception will be thrown.
		/// 
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <returns>the last element in the list</returns>
		static constexpr value_type last_impl(range_type range)
		{
			if(!range.move_next())
				throw sequence_empty_exception();

			auto result = range.get_value();

			while (range.move_next())
			{
				result = range.get_value();
			}
			
			return result;
		}

		/// <summary>
		/// Determines the first element of the range.
		/// If the range is empty, a sequence_empty_exception will be thrown.
		/// 
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="predicate">the predicate which needs to be satisfied by the range values</param>
		/// <returns>the last element in the list</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		static constexpr value_type last_impl(range_type range, const TPredicate & predicate)
		{
			if (!range.move_next())
				throw sequence_empty_exception();

			auto result = range.get_value();
			bool updated = false;

			while (range.move_next())
			{
				const auto value = range.get_value();
				
				if(predicate(value))
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
		/// Determines the last element of the range.
		/// If the range is empty, the default will be returned
		/// 
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <returns>the last element in the list</returns>
		static constexpr value_type last_or_default_impl(range_type range)
		{
			if (!range.move_next())
				return value_type{};

			auto result = range.get_value();

			while (range.move_next())
			{
				result = range.get_value();
			}
			
			return result;
		}

		/// <summary>
		/// Determines the first element of the range.
		/// If the range is empty or not values satisfied the predicate, the default will be returnedd
		/// 
		/// </summary>
		/// <param name="range">the range to operate on</param>
		/// <param name="predicate">the predicate which needs to be satisfied by the range values</param>
		/// <returns>the last element in the list</returns>
		template<typename TPredicate, typename = std::enable_if_t<is_predicate<TPredicate>>>
		static constexpr value_type last_or_default_impl(range_type range, const TPredicate & predicate)
		{
			if (!range.move_next())
				return value_type{};

			auto result = range.get_value();
			bool updated = false;

			while (range.move_next())
			{
				const auto value = range.get_value();

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

		template<range_concept TOtherRange>
		static constexpr bool sequence_equal_impl(range_type lhs, TOtherRange rhs)
		{
			while (true)
			{
				const bool lhs_move = lhs.move_next();
				const bool rhs_move = rhs.move_next();

				if (lhs_move != rhs_move)
					return false;

				if(!lhs_move && !rhs_move)
				{
					return true;
				}

				if(lhs.get_value() != rhs.get_value())
				{
					return false;
				}
			}
		}

		template<range_concept TOtherRange, typename TPredicate, typename = std::enable_if_t<std::is_same_v<std::invoke_result_t<TPredicate, value_type, value_type>, bool>>>
		static constexpr bool sequence_equal_impl(range_type lhs, TOtherRange rhs, const TPredicate & predicate)
		{
			while (true)
			{
				const bool lhs_move = lhs.move_next();
				const bool rhs_move = rhs.move_next();

				if (lhs_move != rhs_move)
					return false;

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

		template<typename TChar>
		static constexpr std::basic_string<TChar> concatenate_impl(
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

		template<template<typename, typename> typename TMap, typename TKeySelection, typename = std::enable_if_t<std::is_invocable_v<TKeySelection, value_type>>>
		static constexpr TMap<std::invoke_result_t<TKeySelection, value_type>, value_type> to_map_impl(range_type range, const TKeySelection & key_selection)
		{
			TMap<std::invoke_result_t<TKeySelection, value_type>, value_type> result;
			while (range.move_next())
			{
				const auto value = range.get_value();
				const auto key   = key_selection(value);
				result.insert({key, value});
			}
			return result;
		}

		template<template<typename> typename TSet>
		static constexpr TSet<value_type> to_set_impl(range_type range)
		{
			TSet<value_type> result;
			
			while (range.move_next())
			{
				result.insert(range.get_value());
			}
			
			return result;
		}

		template<template<typename> typename TQueue>
		static constexpr TQueue<value_type> to_queue_impl(range_type range)
		{
			TQueue<value_type> result;

			while (range.move_next())
			{
				result.push(range.get_value());
			}
			
			return result;
		}

		static constexpr std::stack<value_type> to_stack_impl(range_type range)
		{
			std::stack<value_type> result;

			while (range.move_next())
			{
				result.push(range.get_value());
			}

			return result;
		}
	
	private:
		
		/// <summary>
		/// Member attributes
		/// </summary>

		range_type range;
		
	};

	template<container_concept TContainer>
	class lookup_range
	{
	public:

		using container_type = std::remove_cvref_t<TContainer>;
		using iterator_type  = typename container_type::const_iterator;
		using value_type     = typename container_type::value_type;
		using return_type    = const value_type &;
	
	public:
		
		enum iterating_state
		{
			initial,
			iterating,
			end
		};

	public:
		
		constexpr lookup_range()
			: iterator(), container(), state(initial)
		{
		}
		
		constexpr explicit lookup_range(const container_type & container)
			: iterator(), container(container), state(initial)
		{
		}
		
		constexpr return_type get_value() const
		{
			return *this->iterator;
		}

		constexpr bool move_next()
		{
			switch (this->state)
			{
				case initial:
				{
					this->iterator = this->container.begin();
					this->state = iterating;
					return this->iterator != this->container.end();
				}

				case iterating:
				{
					++this->iterator;

					if(this->iterator == this->container.end())
					{
						this->state = end;
						return false;
					}

					return true;
				}

				case end:
				default:
					return false;
			}
		}
	
	protected:

		iterator_type   iterator;
		container_type  container;
		iterating_state state;
		
	};
	
	template<
		range_concept TRange,
		typename TGroupSelector,
		typename TRangeType = std::remove_cvref_t<TRange>,
		typename TGroupSelectorType = std::remove_cvref_t<TGroupSelector>,
		typename TElementType = typename TRangeType::value_type,
		typename TKeyType = std::invoke_result_t<TGroupSelectorType, TElementType>,
		typename TListType = std::list<TElementType>,
		typename TValueType = std::pair<TKeyType, TListType>,
		typename TValuesType = std::list<TValueType>
	>
	class outer_lookup_range : public lookup_range<TValuesType>
	{
	public:

		using range_type          = TRangeType;
		using group_selector_type = TGroupSelectorType;
		using list_type           = TListType;
		using key_type            = TKeyType;
		using value_type          = TValueType;
		using return_type         = const TValueType &;
	
	public:

		constexpr explicit outer_lookup_range(
			range_type range,
			const group_selector_type & selector
		)
		{
			while (range.move_next())
			{
				const auto value = range.get_value();
				const auto key   = selector(value);

				const auto searchItr = std::find_if(this->container.begin(), this->container.end(),
					[&key](const auto & current_pair)
				{
					return current_pair.first == key;
				});

				if(searchItr != this->container.end())
				{
					searchItr->second.push_back(value);
				} else
				{
					this->container.push_back(std::make_pair(key, list_type{value}));
				}
			}

			this->container.sort([](const auto & lhs_pair, const auto & rhs_pair)
			{
				return lhs_pair.first < rhs_pair.first;
			});
		}
		
		constexpr list_type get_values_for_key(const key_type & key) const
		{
			const auto itr = std::find_if(this->container.begin(), this->container.end(), [&key](const auto & current_value)
			{
				return current_value.first == key;
			});

			if(itr != this->container.end())
			{
				return itr->second;
			}

			return {};
		}
	
	};
	
	template<range_concept TRange, typename TGroupSelector>
	class lookup_table : public enumerable<outer_lookup_range<TRange, TGroupSelector>>
	{
	public:
				
		using underlying_range_type = std::remove_cvref_t<TRange>;
		using group_selector_type   = std::remove_cvref_t<TGroupSelector>;
		using element_type          = typename underlying_range_type::value_type;
		using key_type              = std::invoke_result_t<group_selector_type, element_type>;
		using container_type        = std::list<element_type>;
	
	public:

		constexpr explicit lookup_table(
			underlying_range_type range,
			const group_selector_type & selector
		) : enumerable<outer_lookup_range<underlying_range_type, TGroupSelector>>(
				outer_lookup_range<underlying_range_type, TGroupSelector>(range, selector)
			)
		{
		}

		constexpr enumerable<lookup_range<container_type>> operator [] (const key_type & key) const
		{
			const auto & outer_range = this->to_range();
			const auto values = outer_range.get_values_for_key(key);
			
			return enumerable<lookup_range<container_type>>(
				lookup_range<container_type>(values)
			);
		}
			
	};
	
	/// <summary>
	/// Creates an enumerable from
	/// an array
	/// </summary>
	/// <typeparam name="TArray">type of an array element</typeparam>
	/// <param name="array">array instance</param>
	/// <returns>an enumerable of type iterator_range</returns>
	template<typename TArray, int ArraySize>
	constexpr enumerable<iterator_range<typename array_traits<TArray[ArraySize]>::iterator>> from(const TArray(& array)[ArraySize])
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
	constexpr enumerable<iterator_range<typename TContainer::const_iterator>> from(
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
	constexpr enumerable<iterator_range<TIterator>> from(
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
	constexpr enumerable<repeat_range<TValue>> repeat(const TValue & value, size_t repetitions)
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
	constexpr enumerable<empty_range<TValue>> empty()
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
	constexpr enumerable<increment_range<TValue>> range(
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