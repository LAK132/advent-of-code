#include "../main.hpp"

#include "lak/test.hpp"

#include "lak/format.hpp"

#include "lak/dsl/utility.hpp"

namespace
{
	auto read_data()
	{
		constexpr auto newline = *(lak::dsl::one_of_chars<U'\r', U'\n'>);
		constexpr auto row     = +(lak::dsl::replace_char_literal<U'.', false> |
                           lak::dsl::replace_char_literal<U'@', true>);
		constexpr auto parser =
		  +lak::dsl::capture_sequence<lak::dsl::capture<row>, newline>;
		return parser.parse(input_data_view()).UNWRAP().value;
	}
}

BEGIN_TEST(2025_4_1)
{
	lak::array<lak::array<bool>> entries = read_data();

	// DEBUG_EXPR(entries.size());
	// DEBUG_EXPR(entries[0U].size());

	const size_t width  = entries[0U].size();
	const size_t height = entries.size();

	size_t accessible = 0U;

	for (size_t y = 0U; y < height; ++y)
	{
		for (size_t x = 0U; x < width; ++x)
		{
			if (!entries[y][x]) continue;

			size_t adjacent = 0U;

			for (size_t j = y - std::min<size_t>(y, 1U);
			     j < std::min<size_t>(y + 2U, height);
			     ++j)
				for (size_t i = x - std::min<size_t>(x, 1U);
				     i < std::min<size_t>(x + 2U, width);
				     ++i)
					if (!(j == y && i == x) && entries[j][i]) ++adjacent;

			if (adjacent < 4U) ++accessible;
		}
	}

	DEBUG(lak::fmt<"{:d}">(accessible));

	return EXIT_SUCCESS;
}
END_TEST()

BEGIN_TEST(2025_4_2)
{
	lak::array<lak::array<bool>> entries = read_data();

	// DEBUG_EXPR(entries.size());
	// DEBUG_EXPR(entries[0U].size());

	const size_t width  = entries[0U].size();
	const size_t height = entries.size();

	size_t total = 0U;
	lak::array<lak::pair<size_t, size_t>> accessible;

	do
	{
		total += accessible.size();
		// DEBUG(lak::fmt<"{:d} -> {:d}">(accessible.size(), total));
		for (const auto &[x, y] : accessible) entries[y][x] = false;
		accessible.clear();

		for (size_t y = 0U; y < height; ++y)
		{
			for (size_t x = 0U; x < width; ++x)
			{
				if (!entries[y][x]) continue;

				size_t adjacent = 0U;

				for (size_t j = y - std::min<size_t>(y, 1U);
				     j < std::min<size_t>(y + 2U, height);
				     ++j)
					for (size_t i = x - std::min<size_t>(x, 1U);
					     i < std::min<size_t>(x + 2U, width);
					     ++i)
						if (!(j == y && i == x) && entries[j][i]) ++adjacent;

				if (adjacent < 4U) accessible.emplace_back(x, y);
			}
		}
	} while (!accessible.empty());

	DEBUG(lak::fmt<"{:d}">(total));

	return EXIT_SUCCESS;
}
END_TEST()
