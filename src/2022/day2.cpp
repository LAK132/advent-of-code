#include "../main.hpp"

#include "lak/test.hpp"

#include "lak/algorithm.hpp"
#include "lak/tokeniser.hpp"

namespace
{
	struct game_round
	{
		char8_t them;
		char8_t you;
	};

	lak::array<game_round> read_data()
	{
		lak::array<game_round> rounds;

		lak::u8tokeniser tokeniser{input_data_view(),
		                           lak::array<lak::u32string>{}};

		char8_t last = u8'\0';
		for (const auto &[token, position] : tokeniser)
		{
			if (last == u8'\0')
			{
				last = token[0];
			}
			else
			{
				rounds.push_back({last, token[0]});
				last = u8'\0';
			}
		}

		return rounds;
	}
}

BEGIN_TEST(2022_2_1)
{
	auto rounds{read_data()};

	auto calc_score = [](const game_round &r) -> lak::result<uintmax_t>
	{
		switch (r.them)
		{
			case u8'A': // rock
				switch (r.you)
				{
					case u8'X': // rock
						return lak::ok_t{/*rock*/ 1 + /*draw*/ 3};
					case u8'Y': // paper
						return lak::ok_t{/*paper*/ 2 + /*win*/ 6};
					case u8'Z': // scissors
						return lak::ok_t{/*scissors*/ 3 + /*lose*/ 0};
					default: return lak::err_t{};
				}

			case u8'B': // paper
				switch (r.you)
				{
					case u8'X': // rock
						return lak::ok_t{/*rock*/ 1 + /*lose*/ 0};
					case u8'Y': // paper
						return lak::ok_t{/*paper*/ 2 + /*draw*/ 3};
					case u8'Z': // scissors
						return lak::ok_t{/*scissors*/ 3 + /*win*/ 6};
					default: return lak::err_t{};
				}

			case u8'C': // scissors
				switch (r.you)
				{
					case u8'X': // rock
						return lak::ok_t{/*rock*/ 1 + /*win*/ 6};
					case u8'Y': // paper
						return lak::ok_t{/*paper*/ 2 + /*lose*/ 0};
					case u8'Z': // scissors
						return lak::ok_t{/*scissors*/ 3 + /*draw*/ 3};
					default: return lak::err_t{};
				}

			default: return lak::err_t{};
		}
	};

	uintmax_t score = 0;
	for (const auto &round : rounds) score += calc_score(round).UNWRAP();
	DEBUG_EXPR(score);

	return EXIT_SUCCESS;
}
END_TEST()

BEGIN_TEST(2022_2_2)
{
	auto rounds{read_data()};

	auto calc_score = [](const game_round &r) -> lak::result<uintmax_t>
	{
		char8_t you;
		switch (r.them)
		{
			case u8'A': // rock
				switch (r.you)
				{
					case u8'X': // lose
						you = u8'Z';
						break;
					case u8'Y': // draw
						you = u8'X';
						break;
					case u8'Z': // win
						you = u8'Y';
						break;
					default: return lak::err_t{};
				}
				break;

			case u8'B': // paper
				switch (r.you)
				{
					case u8'X': // lose
						you = u8'X';
						break;
					case u8'Y': // draw
						you = u8'Y';
						break;
					case u8'Z': // win
						you = u8'Z';
						break;
					default: return lak::err_t{};
				}
				break;

			case u8'C': // scissors
				switch (r.you)
				{
					case u8'X': // lose
						you = u8'Y';
						break;
					case u8'Y': // draw
						you = u8'Z';
						break;
					case u8'Z': // win
						you = u8'X';
						break;
					default: return lak::err_t{};
				}
				break;

			default: return lak::err_t{};
		}

		switch (r.them)
		{
			case u8'A': // rock
				switch (you)
				{
					case u8'X': // rock
						return lak::ok_t{/*rock*/ 1 + /*draw*/ 3};
					case u8'Y': // paper
						return lak::ok_t{/*paper*/ 2 + /*win*/ 6};
					case u8'Z': // scissors
						return lak::ok_t{/*scissors*/ 3 + /*lose*/ 0};
					default: return lak::err_t{};
				}

			case u8'B': // paper
				switch (you)
				{
					case u8'X': // rock
						return lak::ok_t{/*rock*/ 1 + /*lose*/ 0};
					case u8'Y': // paper
						return lak::ok_t{/*paper*/ 2 + /*draw*/ 3};
					case u8'Z': // scissors
						return lak::ok_t{/*scissors*/ 3 + /*win*/ 6};
					default: return lak::err_t{};
				}

			case u8'C': // scissors
				switch (you)
				{
					case u8'X': // rock
						return lak::ok_t{/*rock*/ 1 + /*win*/ 6};
					case u8'Y': // paper
						return lak::ok_t{/*paper*/ 2 + /*lose*/ 0};
					case u8'Z': // scissors
						return lak::ok_t{/*scissors*/ 3 + /*draw*/ 3};
					default: return lak::err_t{};
				}

			default: return lak::err_t{};
		}
	};

	uintmax_t score = 0;
	for (const auto &round : rounds) score += calc_score(round).UNWRAP();
	DEBUG_EXPR(score);

	return EXIT_SUCCESS;
}
END_TEST()
