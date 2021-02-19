#pragma once
#include <string>
#include <algorithm>
#include <sstream>

class crypto {
private:
  static int char_to_value(char c);
  static int value_to_char(int num);

  static std::string to_decimal(std::string src, int base_from);
  static std::string from_decimal(std::string src, int base_to);

public:
  static std::string remove_spaces(std::string src);
  static std::string add_spaces(std::string src, std::string seria_length);

  static std::string base_convert(std::string src, std::string base_from, std::string base_to, std::string prepending_zeroes = "");

  static std::string ascii_encode(std::string src);
  static std::string ascii_decode(std::string src);

  static std::string rle_encode(std::string src);
  static std::string rle_decode(std::string src);

  static std::string hem_encode(std::string src);
  static std::string hem_decode(std::string src);
};
