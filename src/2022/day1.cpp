#include "../main.hpp"

#include "lak/test.hpp"

#include "lak/algorithm.hpp"
#include "lak/tokeniser.hpp"

namespace
{
	struct elf
	{
		lak::array<uintmax_t> calories;
		uintmax_t total;
	};

	bool operator<(const elf &lhs, const elf &rhs)
	{
		return lhs.total < rhs.total;
	}

	bool operator>(const elf &lhs, const elf &rhs)
	{
		return lhs.total > rhs.total;
	}

	lak::array<elf> read_data()
	{
		auto lines{split_lines(input_data_view())};

		lak::array<elf> elves;

		{
			lak::array<uintmax_t> c;
			uintmax_t t = 0;

			for (const auto &line : lines)
			{
				if (line.empty())
				{
					elves.push_back({lak::move(c), t});
					c.clear();
					t = 0;
				}
				else
				{
					uintmax_t v = parse_decimal(line).UNWRAP();
					t += v;
					c.push_back(v);
				}
			}

			if (!c.empty())
			{
				elves.push_back({lak::move(c), t});
			}
		}

		return elves;
	}
}

BEGIN_TEST(2022_1_1)
{
	auto elves{read_data()};

	auto max_elf{lak::max_element(elves.begin(), elves.end())};

	DEBUG_EXPR(max_elf->total);

	return EXIT_SUCCESS;
}
END_TEST()

BEGIN_TEST(2022_1_2)
{
	auto elves{read_data()};

	lak::heapsort(elves.begin(), elves.end(), lak::greater<>{});

	DEBUG_EXPR(elves[0].total + elves[1].total + elves[2].total);

	return EXIT_SUCCESS;
}
END_TEST()
