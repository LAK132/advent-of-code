#include "../main.hpp"

#include "lak/test.hpp"

#include "lak/format.hpp"

#include "lak/dsl/utility.hpp"

namespace
{
	auto read_data()
	{
		constexpr auto newline = *(lak::dsl::one_of_chars<U'\r', U'\n'>);
		constexpr auto end_of_entry =
		  (lak::dsl::char_literal<U','> + newline) | (newline + lak::dsl::eof);
		constexpr auto parser = +lak::dsl::capture_sequence<
		  lak::dsl::capture<lak::dsl::parsed_dec_uint<uint64_t>>,
		  lak::dsl::char_literal<U'-'>,
		  lak::dsl::capture<lak::dsl::parsed_dec_uint<uint64_t>>,
		  end_of_entry>;
		return parser.parse(input_data_view()).UNWRAP().value;
	}
}

BEGIN_TEST(2025_2_1)
{
	auto entries = read_data();

	// DEBUG_EXPR(entries.size());

	auto is_invalid_id = [](lak::u8string_view str) -> bool
	{
		if ((str.size() % 2U) != 0U) return false;
		auto [first, second] = lak::split(lak::span(str), str.size() / 2U);
		ASSERT_EQUAL(first.size(), second.size());
		return lak::compare(first, second) == first.size();
	};

	uint64_t total = 0U;

	for (const auto &[first, last] : entries)
	{
		for (uint64_t i = first; i <= last; ++i)
		{
			auto str = lak::fmt<u8"{:d}">(i);
			if (is_invalid_id(str))
			{
				total += i;
				DEBUG(lak::fmt<"{} -> {:d}">(str, total));
			}
		}
	}

	DEBUG(lak::fmt<"total: {:d}">(total));

	return EXIT_SUCCESS;
}
END_TEST()

BEGIN_TEST(2025_2_2)
{
	auto entries = read_data();

	// DEBUG_EXPR(entries.size());

	auto is_invalid_id = [](lak::u8string_view str) -> bool
	{
		for (size_t i = 1U; i <= (str.size() / 2U); ++i)
		{
			if ((str.size() % i) != 0U) continue;
			auto [substr, rest] = lak::split(lak::span(str), i);
			while (lak::compare(substr, rest) == substr.size())
				lak::tie(substr, rest) = lak::split(rest, i);
			if (rest.empty()) return true;
		}

		return false;
	};

	uint64_t total = 0U;

	for (const auto &[first, last] : entries)
	{
		for (uint64_t i = first; i <= last; ++i)
		{
			auto str = lak::fmt<u8"{:d}">(i);
			if (is_invalid_id(str))
			{
				total += i;
				// DEBUG(lak::fmt<"{} -> {:d}">(str, total));
			}
		}
	}

	DEBUG(lak::fmt<"total: {:d}">(total));

	return EXIT_SUCCESS;
}
END_TEST()
