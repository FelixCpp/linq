#pragma once

#include <concepts>

namespace linq
{
	template<typename TRange>
	concept range_concept = requires(TRange range)
	{
		typename TRange::value_type;
		typename TRange::return_type;

		{ range.get_value() } -> std::convertible_to<typename TRange::return_type>;
		{ range.move_next() } -> std::convertible_to<bool>;
	};

	template<typename TContainer>
	concept container_concept = requires(const TContainer & container)
	{
		typename TContainer::const_iterator;
		typename TContainer::value_type;
		container.begin();
		container.end();
	};

	template<typename TRange>
	concept sorting_range_concept = range_concept<TRange> && requires(TRange range)
	{
		{ range.compare_values(typename TRange::value_type{}, typename TRange::value_type{}) } -> std::same_as<bool>;
		{ range.forward_get_value() } -> std::same_as<typename TRange::return_type>;
		{ range.forward_move_next() } -> std::same_as<bool>;
	};
}