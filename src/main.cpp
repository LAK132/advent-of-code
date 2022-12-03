#include "main.hpp"

#include "lak/char_utils.hpp"
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
	              "\nexample: aoc 2021_3_1 data/2021/day3.txt" LAK_SGR_RESET);

	{
		auto file = lak::read_file(argv[2]).UNWRAP();
		input_data.resize(file.size());
		lak::memcpy(lak::span<byte_t>(lak::span<char8_t>(input_data)),
		            lak::span<const byte_t>{file});
	}

	lak::run_tests(lak::as_u8string(lak::astring_view::from_c_str(argv[1])));

	return 0;
}

lak::u8string_view input_data_view()
{
	return lak::u8string_view{input_data.begin(), input_data.end()};
}

lak::array<lak::u8string_view> split_lines(lak::u8string_view str)
{
	lak::array<lak::u8string_view> result;

	const char8_t *begin = str.begin();
	const char8_t *end   = begin;

	for (const char8_t &c : str)
	{
		if (c == u8'\n')
		{
			if (!begin)
				result.push_back(lak::u8string_view{&c, &c});
			else
				result.push_back(lak::u8string_view{begin, end + 1});
			begin = end = nullptr;
		}
		else if (c == u8'\r')
		{
		}
		else
		{
			end = &c;
			if (!begin) begin = end;
		}
	}

	if (begin) result.push_back(lak::u8string_view{begin, end + 1});

	return result;
}

lak::result<uintmax_t> parse_binary(lak::u8string_view str)
{
	// ([01]+)

	auto binary_from_alphanumberic = [](char8_t c) -> lak::result<uint8_t>
	{
		if (c != u8'0' && c != u8'1') return lak::err_t{};
		return lak::ok_t<uint8_t>{uint8_t(c - u8'0')};
	};

	if (str.empty()) return lak::err_t{};

	auto *iter = str.begin();

	uintmax_t result = 0;

	auto v = binary_from_alphanumberic(*(iter++));

	if (v.is_err())
		return lak::err_t{}; // not binary alphanumeric
	else
		result = uintmax_t(v.unsafe_unwrap());

	while (iter != str.end())
	{
		if_let_ok(const uint8_t new_digit, binary_from_alphanumberic(*iter))
		{
			++iter;

			const uintmax_t new_result = result << 1;
			if (uintmax_t(new_result >> 1) != result)
				return lak::err_t{}; // integer too large

			result = new_result | new_digit;
		}
		else break; // not binary alphanumeric
	}

	return lak::ok_t<uintmax_t>{result};
}

lak::result<uintmax_t> parse_decimal(lak::u8string_view str)
{
	// ([0-9]+)

	if (str.empty()) return lak::err_t{};

	auto *iter = str.begin();

	uintmax_t result = 0;

	auto v = lak::from_alphanumeric(*(iter++));

	if (v.is_err())
		return lak::err_t{}; // not alphanumeric
	else
		result = uintmax_t(v.unsafe_unwrap());

	while (iter != str.end())
	{
		if_let_ok(const uint8_t new_digit, lak::from_alphanumeric(*iter))
		{
			++iter;

			const uintmax_t new_result = result * 10;
			if (uintmax_t(new_result / 10) != result)
				return lak::err_t{}; // integer too large

			if (new_digit > (UINTMAX_MAX - new_result))
				return lak::err_t{}; // integer too large

			result = new_result + new_digit;
		}
		else break; // not alphanumeric
	}

	return lak::ok_t<uintmax_t>{result};
}

lak::result<intmax_t> parse_signed_decimal(lak::u8string_view str)
{
	// (-?[0-9]+)

	if (str.empty()) return lak::err_t{};

	bool is_negative = str[0] == u8'-';
	if (is_negative) str = str.substr(1);

	if (auto result = parse_decimal(str); result.is_ok())
	{
		if (is_negative)
			return lak::ok_t<intmax_t>{-intmax_t(result.unsafe_unwrap())};
		else
			return lak::ok_t<intmax_t>{intmax_t(result.unsafe_unwrap())};
	}
	else
		return lak::err_t{};
}

lak::result<uintmax_t> parse_hexadecimal(lak::u8string_view str)
{
	// ([0-9a-fA-F]+)

	if (str.empty()) return lak::err_t{};

	auto *iter = str.begin();

	uintmax_t result = 0;

	auto v = lak::from_hex_alphanumeric(*(iter++));

	if (v.is_err())
		return lak::err_t{}; // not alphanumeric
	else
		result = uintmax_t(v.unsafe_unwrap());

	while (iter != str.end())
	{
		if_let_ok(const uint8_t new_digit, lak::from_hex_alphanumeric(*iter))
		{
			++iter;

			const uintmax_t new_result = result << 4;
			if (uintmax_t(new_result >> 4) != result)
				return lak::err_t{}; // integer too large

			result = new_result | new_digit;
		}
		else break; // not alphanumeric
	}

	return lak::ok_t<uintmax_t>{result};
}
