#pragma once

#include <type_traits>

namespace linq
{

	template<typename TPredicate, typename ... TArgs>
	struct is_predicate
	{
		using predicate_type = std::remove_cvref_t<TPredicate>;
		using result_type    = std::invoke_result_t<predicate_type, TArgs ...>;
		
		inline static bool is_invocable = std::is_invocable_v<predicate_type, TArgs ...>;
		inline static bool returns_bool = std::is_same_v<result_type, bool>;
		inline static bool value        = is_invocable && returns_bool;
	};

	template<typename TAction, typename ... TArgs>
	struct is_action
	{
		using predicate_type = std::remove_cvref_t<TAction>;
		using result_type    = std::invoke_result_t<predicate_type, TArgs ...>;

		inline static bool is_invocable = std::is_invocable_v<predicate_type, TArgs ...>;
		inline static bool returns_void = std::is_same_v<result_type, void>;
		inline static bool value        = is_invocable && returns_void;
	};
	
	template<typename TCallable, typename ... TArgs>
	struct is_callable
	{
		using predicate_type = std::remove_cvref_t<TCallable>;
		using result_type    = std::invoke_result_t<predicate_type, TArgs ...>;

		inline static bool is_invocable = std::is_invocable_v<predicate_type, TArgs ...>;
		inline static bool value        = is_invocable;
	};

	template<typename TPredicate, typename ... TArgs> inline static bool is_predicate_v = is_predicate<TPredicate, TArgs ...>::value;
	template<typename TAction, typename ... TArgs>    inline static bool is_action_v    = is_action<TAction, TArgs ...>::value;
	template<typename TCallable, typename ... TArgs>  inline static bool is_callable_v  = is_callable<TCallable, TArgs ...>::value;
	
	
	
}