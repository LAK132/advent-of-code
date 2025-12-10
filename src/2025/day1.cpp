#include "../main.hpp"

#include "lak/test.hpp"

#include "lak/format.hpp"

#include "lak/dsl/utility.hpp"

namespace
{
	lak::array<lak::tuple<bool, uint32_t>> read_data()
	{
		constexpr auto parser = +lak::dsl::capture_1st<
		  (lak::dsl::replace_char_literal<U'L', true> |
		   lak::dsl::replace_char_literal<U'R', false>)+lak::dsl::
		    parsed_dec_uint<uint32_t>,
		  (+(lak::dsl::one_of_chars<U'\r', U'\n'>) | lak::dsl::eof)>;

		auto commands = parser.parse(input_data_view()).UNWRAP().value;

		return commands;
	}
}

BEGIN_TEST(2025_1_1)
{
	auto commands = read_data();

	// DEBUG_EXPR(commands.size());

	intmax_t pos        = 50;
	uint32_t zero_count = 0;

	for (const auto &[left, count] : commands)
	{
		if (left)
			pos -= count;
		else
			pos += count;
		while (pos < 0) pos += 100;
		while (pos > 99) pos -= 100;
		if (pos == 0) ++zero_count;

		// DEBUG(lak::fmt<"{}{:d}  \t-> {:d}">(
		//   (const char *)(left ? "L" : "R"), count, pos));
	}

	DEBUG(lak::fmt<"ZC: {}">(zero_count));

	return EXIT_SUCCESS;
}
END_TEST()

BEGIN_TEST(2025_1_2)
{
	auto commands = read_data();

	// DEBUG_EXPR(commands.size());

	intmax_t pos        = 50;
	uint32_t zero_count = 0;

	for (auto [left, count] : commands)
	{
		zero_count += count / 100U;
		count %= 100U;

		if (left)
			pos -= count;
		else
			pos += count;

		if (pos == 0)
			++zero_count;
		else if (pos < 0)
		{
			if (pos + count != 0) ++zero_count;
			pos += 100;
		}
		else if (pos > 99)
		{
			pos -= 100;
			++zero_count;
		}

		// DEBUG(lak::fmt<"{}{:d}  \t-> {:d}">(
		//   (const char *)(left ? "L" : "R"), count, pos));
	}

	DEBUG(lak::fmt<"ZC: {}">(zero_count));

	return EXIT_SUCCESS;
}
END_TEST()
