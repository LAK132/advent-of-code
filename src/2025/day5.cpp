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
		constexpr auto id    = lak::dsl::parsed_dec_uint<uint64_t>;
		constexpr auto range = lak::dsl::transform<
		  lak::dsl::capture_sequence<lak::dsl::capture<id>,
		                             lak::dsl::char_literal<U'-'>,
		                             lak::dsl::capture<id>>,
		  [](lak::tuple<uint64_t, uint64_t> val) -> lak::pair<uint64_t, uint64_t>
		  {
			  auto [f, s] = val;
			  return {f, s};
		  }>;
		constexpr auto parser =
		  (+lak::dsl::capture_1st<range, eol>)+(+lak::dsl::capture_1st<id, eol>);
		return parser.parse(input_data_view()).UNWRAP().value;
	}
}

BEGIN_TEST(2025_5_1)
{
	auto [ranges, ids] = read_data();

	// DEBUG_EXPR(ranges.size());
	// DEBUG_EXPR(ids.size());

	size_t fresh = 0U;

	for (const auto &id : ids)
	{
		for (const auto &[min, max] : ranges)
		{
			if (id >= min && id <= max)
			{
				// DEBUG(lak::fmt<"{:d} <= {:d} <= {:d}">(min, id, max));
				++fresh;
				break;
			}
		}
	}

	DEBUG(lak::fmt<"fresh: {:d}">(fresh));

	return EXIT_SUCCESS;
}
END_TEST()

BEGIN_TEST(2025_5_2)
{
	auto [ranges, ids] = read_data();
	LAK_UNUSED(ids);

	using range_t = lak::pair<uint64_t, uint64_t>;

	auto less = lak::less<range_t, &range_t::first, &range_t::second>{};

	lak::heapsort(ranges.begin(), ranges.end(), less);

	ranges.erase(lak::stable_dedup(ranges.begin(),
	                               ranges.end(),
	                               [](range_t &lhs, const range_t &rhs) -> bool
	                               {
		                               if (lhs.second < rhs.first) return false;
		                               lhs.second = std::max<uint64_t>(lhs.second,
		                                                               rhs.second);
		                               return true;
	                               }),
	             ranges.end());

	uint64_t total = 0U;

	for (const auto &[min, max] : ranges) total += (max + 1U) - min;

	DEBUG(lak::fmt<"fresh: {:d}">(total));

	return EXIT_SUCCESS;
}
END_TEST()
