#include "lak/array.hpp"

extern lak::array<char8_t> input_data;

lak::u8string_view input_data_view();

lak::array<lak::u8string_view> split_lines(lak::u8string_view str);

lak::result<uintmax_t> parse_binary(lak::u8string_view str);

lak::result<uintmax_t> parse_decimal(lak::u8string_view str);

lak::result<intmax_t> parse_signed_decimal(lak::u8string_view str);

lak::result<uintmax_t> parse_hexadecimal(lak::u8string_view str);
