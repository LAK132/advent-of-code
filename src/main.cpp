#include "main.hpp"

#include "lak/debug.hpp"
#include "lak/file.hpp"
#include "lak/span.hpp"
#include "lak/test.hpp"

lak::array<char8_t> input_data;

int main(int argc, const char **argv)
{
	ASSERTF_EQUAL(argc,
	              3,
	              LAK_ITALIC LAK_BLUE
	              "\nusage: aoc <day> <path to input data>"
	              "\nexample: aoc day3part2 2021day3.txt" LAK_SGR_RESET);

	{
		auto file = lak::read_file(argv[2]).UNWRAP();
		input_data.resize(file.size());
		lak::memcpy(lak::span<byte_t>(lak::span<char8_t>(input_data)),
		            lak::span<const byte_t>{file});
	}

	lak::run_tests(lak::as_u8string(lak::astring_view::from_c_str(argv[1])));

	return 0;
}
