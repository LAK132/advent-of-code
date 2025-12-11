#include "../main.hpp"

#include "lak/test.hpp"

#include "lak/format.hpp"

#include "lak/dsl/utility.hpp"

#include <cmath>

namespace
{
	auto read_data()
	{
		constexpr auto newline = *(lak::dsl::one_of_chars<U'\r', U'\n'>);
		constexpr auto bank    = +(lak::dsl::parsed_uint<lak::dsl::dec_digit,
		                                                 lak::numeric_base::dec,
		                                                 uint8_t>);
		constexpr auto parser =
		  +lak::dsl::capture_sequence<lak::dsl::capture<bank>, newline>;
		return parser.parse(input_data_view()).UNWRAP().value;
	}

	uint64_t max_joltage(lak::span<const uint8_t> bank, uint32_t count)
	{
		uint64_t result = 0U;
		size_t index    = lak::dynamic_extent;

		for (size_t i = 0; i <= bank.size() - count; ++i)
		{
			if (bank[i] > result)
			{
				result = bank[i];
				index  = i;
			}
		}

		ASSERT_NOT_EQUAL(index, lak::dynamic_extent);

		if (count > 1)
			return (result * uint64_t(std::pow(10U, count - 1U))) +
			       max_joltage(bank.subspan(index + 1U), count - 1U);
		else
			return result;
	}
}

BEGIN_TEST(2025_3_1)
{
	auto entries = read_data();

	// DEBUG_EXPR(entries.size());

	uint64_t result = 0;

	for (const auto &bank : entries)
	{
		auto joltage = max_joltage(bank, 2U);
		result += joltage;
		// DEBUG(lak::fmt<"{:d} -> {:d}">(joltage, result));
	}

	DEBUG(lak::fmt<"{:d}">(result));

	return EXIT_SUCCESS;
}
END_TEST()

BEGIN_TEST(2025_3_2)
{
	auto entries = read_data();

	// DEBUG_EXPR(entries.size());

	uint64_t result = 0;

	for (const auto &bank : entries)
	{
		auto joltage = max_joltage(bank, 12U);
		result += joltage;
		// DEBUG(lak::fmt<"{:d} -> {:d}">(joltage, result));
	}

	DEBUG(lak::fmt<"{:d}">(result));

	return EXIT_SUCCESS;
}
END_TEST()
