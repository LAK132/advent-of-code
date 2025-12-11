#include "../main.hpp"

#include "lak/test.hpp"

#include "lak/format.hpp"

#include "lak/dsl/utility.hpp"

namespace
{
	auto read_data()
	{
		constexpr auto eol =
		  +(lak::dsl::one_of_chars<U'\r', U'\n'>) | lak::dsl::eof;
		constexpr auto values = lak::dsl::capture_2nd<
		  *lak::dsl::char_literal<U' '>,
		  +lak::dsl::capture_1st<lak::dsl::parsed_dec_uint<uint64_t>,
		                         *lak::dsl::char_literal<U' '>>,
		  +lak::dsl::one_of_chars<U'\r', U'\n'>>;
		constexpr auto operations = lak::dsl::capture_2nd<
		  *lak::dsl::char_literal<U' '>,
		  +lak::dsl::capture_1st<(lak::dsl::replace_char_literal<U'*', true> |
		                          lak::dsl::replace_char_literal<U'+', false>),
		                         *lak::dsl::char_literal<U' '>>,
		  eol>;
		constexpr auto parser = +values + operations;
		return parser.parse(input_data_view()).UNWRAP().value;
	}
}

BEGIN_TEST(2025_6_1)
{
	auto [values, operations] = read_data();

	// DEBUG_EXPR(values.size());
	// DEBUG_EXPR(operations.size());

	uint64_t result = 0U;

	for (size_t i = 0U; i < operations.size(); ++i)
	{
		auto value{values[0][i]};
		if (operations[i])
			for (size_t j = 1U; j < values.size(); ++j) value *= values[j][i];
		else
			for (size_t j = 1U; j < values.size(); ++j) value += values[j][i];
		result += value;
		// DEBUG(lak::fmt<"{:d} -> {:d}">(value, result));
	}

	DEBUG(lak::fmt<"sum: {:d}">(result));

	return EXIT_SUCCESS;
}
END_TEST()

BEGIN_TEST(2025_6_2)
{
	auto lines{split_lines(input_data_view())};

	// DEBUG_EXPR(lines.size());

	auto values{lak::span(lines).first(lines.size() - 1U)};
	auto operations{lines.back()};

	// DEBUG_EXPR(values.size());
	// DEBUG_EXPR(operations.size());

	constexpr auto numbers_parser =
	  +lak::dsl::capture_2nd<*lak::dsl::char_literal<U' '>,
	                         lak::dsl::parsed_dec_uint<uint64_t>>;

	constexpr auto operations_parser =
	  +((lak::dsl::replace_char_literal<U'*', true> |
	     lak::dsl::replace_char_literal<U'+', false>)+lak::dsl::
	      transform<+lak::dsl::char_literal<U' '>,
	                [](lak::u8string_view s) -> size_t { return s.size(); }>);
	auto ops = operations_parser.parse(operations).UNWRAP().value;

	uint64_t result = 0U;

	for (const auto &[op, count] : ops)
	{
		ASSERT_NOT_EQUAL(count, 0U); // needs padding on end of line

		lak::u8string numbers;
		for (size_t i = 0U; i <= count; ++i)
		{
			for (size_t j = 0U; j < values.size(); ++j)
			{
				numbers += values[j][0];
				values[j] = values[j].substr(1U);
			}
			numbers += u8" ";
		}

		// DEBUG_EXPR(numbers);

		auto nums = numbers_parser.parse(numbers).UNWRAP().value;

		auto value{nums[0]};
		if (op)
			for (size_t j = 1U; j < nums.size(); ++j) value *= nums[j];
		else
			for (size_t j = 1U; j < nums.size(); ++j) value += nums[j];
		result += value;
		// DEBUG(lak::fmt<"{:d} -> {:d}">(value, result));
	}

	DEBUG(lak::fmt<"sum: {:d}">(result));

	return EXIT_SUCCESS;
}
END_TEST()
