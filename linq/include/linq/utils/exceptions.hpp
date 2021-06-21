#pragma once

#include <stdexcept>

namespace linq
{

	/// <summary>
	/// Invalid_Operation_Exception definition (derives from std::exception)
	/// since it should be the base class for all
	/// exceptions
	/// </summary>
	struct invalid_operation_exception final : std::exception
	{
		using std::exception::exception;

		invalid_operation_exception()
			: exception("invalid_operation_exception")
		{
		}
	};

	/// <summary>
	/// Sequence_Empty_Exception definition (derives from std::exception)
	/// since it should be the base class for all
	/// exceptions
	/// </summary>
	struct sequence_empty_exception final : std::exception
	{
		using std::exception::exception;

		sequence_empty_exception()
			: exception("sequence_empty_exception")
		{
		}
	};

	/// <summary>
	/// Index_Out_Of_Bounds_Exception definition (derives from std::exception)
	/// since it should be the base class for all
	/// exceptions
	/// </summary>
	struct index_out_of_bounds_exception final : std::exception
	{
		using std::exception::exception;

		index_out_of_bounds_exception()
			: exception("index_out_of_bounds_exception")
		{
		}
	};
	
}