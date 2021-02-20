#include "crypto.h"

int crypto::char_to_value(char c) {
  if (c >= '0' && c <= '9')
    return (int)c - '0';
  else
    return (int)c - 'A' + 10;
}

int crypto::value_to_char(int num)
{
  if (num >= 0 && num <= 9)
    return (num + '0');
  else
    return (num - 10 + 'A');
}

std::string crypto::to_decimal(std::string src, int base_from) {
  int res = 0;

  int power = 1;
  for (int i = src.length() - 1; i >= 0; i--) {
    if (char_to_value(src[i]) >= base_from)
      return ":(";

    res += char_to_value(src[i]) * power;
    power = power * base_from;
  }

  return std::to_string(res);
}

std::string crypto::from_decimal(std::string src, int base_to) {
  std::string res;

  while (stoi(src) > 0) {
    res.push_back((char)value_to_char(stoi(src) % base_to));
    src = std::to_string(stoi(src) / base_to);
  }

  std::reverse(res.begin(), res.end());

  if (res.empty())
    res.push_back('0');
  return res;
}

std::string crypto::remove_spaces(std::string src) {
  std::string res;

  for (int i = 0; i < (int)src.size(); i++)
    if (src[i] != ' ')
      res.push_back(src[i]);

  return res;
}

std::string crypto::add_spaces(std::string src, std::string seria_length) {
  std::string res;

  for (int i = 0; i < (int)src.size(); i++) {
    res.push_back(src[i]);
    if ((i + 1) % std::stoi(seria_length) == 0 && i && i != src.size() - 1)
      res.push_back(' ');
  }
  
  return res;
}

std::string crypto::base_convert(std::string src, std::string base_from, std::string base_to, std::string prepending_zeroes) {
  std::string res, seria;
  std::stringstream series(src);

  while (series >> seria) {
    std::string tmp_res = to_decimal(seria, std::stoi(base_from));
    tmp_res = from_decimal(tmp_res, std::stoi(base_to));

    if (prepending_zeroes != "")
      while (tmp_res.size() % std::stoi(prepending_zeroes))
        tmp_res = '0' + tmp_res;
      
    res.append(tmp_res);
  }

  return res;
}

std::string crypto::ascii_encode(std::string src) {
  std::string res;

  while (src.size() % 8)
    src.erase(src.end() - 1, src.end());

  std::string binary8;
  for (int i = 0; i < (int)src.size(); i++) {
    binary8.push_back(src[i]);

    if (binary8.size() == 8) {
      res.push_back((char)std::stoi(base_convert(binary8, "2", "10")));
      binary8.clear();
    }
  }

  return res;
}


std::string crypto::ascii_decode(std::string src) {
  std::string res;

  std::string binary8;
  for (int i = 0; i < (int)src.size(); i++) {
    binary8 = base_convert(std::to_string((int)src[i] + 256), "10", "2");
    while (binary8.size() < 8)
      binary8 = '0' + binary8;

    res.append(binary8);
  }

  return res;
}

std::string crypto::base64_encode(std::string src) {
  std::string src_binary = ascii_decode(src);

  int equal_count = ((6 - src_binary.size() % 6) % 6) / 2;
  src_binary.append(std::string(equal_count * 2, '0'));

  const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string res, binary6;
  for (int i = 0; i < src_binary.size(); i++) {
    binary6.push_back(src_binary[i]);
    if (binary6.size() == 6) {
      res.push_back(alphabet[std::stoi(base_convert(binary6, "2", "10"))]);
      binary6.clear();
    }
  }

  res.append(std::string(equal_count, '='));
  return res;
}

std::string crypto::base64_decode(std::string src) {
  std::string src_binary;
  const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int equal_count = 0;
  for (int i = 0; i < src.size(); i++) 
    if (src[i] == '=') {
      equal_count++;
      continue;
    }
    else 
      src_binary.append(base_convert(std::to_string(alphabet.find(src[i])), "10", "2", "6"));
    
  for (int i = 0; i < equal_count; i++)
    src_binary.erase(src_binary.end() - 2, src_binary.end());

  std::string res, binary8;
  for (int i = 0; i < src_binary.size(); i++) {
    binary8.push_back(src_binary[i]);
    if (binary8.size() == 8) {
      res.append(ascii_encode(binary8));
      binary8.clear();
    }
  }

  return res;
}

std::string crypto::rle_encode(std::string src) {
  std::string src_series;
  char last_symbol = src[0];
  src_series.push_back(last_symbol);
  for (int i = 1; i < (int)src.size(); i++) {
    if (src[i] != last_symbol) 
      src_series.push_back(' ');
    src_series.push_back(src[i]);
    last_symbol = src[i];
  }

  std::string res, seria, gamma;
  res.push_back(src_series[0]);
  std::stringstream series(src_series);
  while (series >> seria) {
    gamma = base_convert(std::to_string(seria.size()), "10", "2");
    for (int i = 0; i < (int)gamma.size() - 1; i++)
      res.push_back('0');
    res.append(gamma);
  }

  return res;
}

std::string crypto::rle_decode(std::string src) {
  std::string src_gamma;
  src_gamma.push_back(src[0]);
  src_gamma.push_back(' ');

  bool type = false; // false: counting prepending zeroes, true: searching data
  int null_count = 0;
  for (int i = 1; i < (int)src.size(); i++) {
    if (!type) {
      if (src[i] == '0') {
        null_count++;
      }
      else {
        src_gamma.push_back(src[i]);
        type = true;
      }
    }
    else {
      if (!null_count) {
        src_gamma.push_back(' ');
        type = false;
        i--;
      }
      else {
        src_gamma.push_back(src[i]);
        null_count--;
      }
    }
  } 

  std::string res, gamma_now;
  std::stringstream gamma_words(src_gamma);
  char bit_now;
  gamma_words >> bit_now;
  int length_now;
  while (gamma_words >> gamma_now) {
    length_now = std::stoi(base_convert(gamma_now, "2", "10"));

    for (int i = 0; i < length_now; i++)
      res.push_back(bit_now);

    bit_now = '0' + '1' - bit_now;
  }

  return res;
}

std::string crypto::hem_encode(std::string src) {
  while (src.size() % 11)
    src.push_back('0');

  std::string res;
  for (int q = 0; q < (int)src.size() / 11; q++) {
    std::string tmp_res(15, '0'), tmp_src(src, q * 11, 11);

    for (int res_i = 0, src_i = 0; res_i < 15; res_i++)
      if (res_i != 0 && res_i != 1 && res_i != 3 && res_i != 7) {
        tmp_res[res_i] = tmp_src[src_i];
        src_i++;
      }

    int tmp_sum = 0;
    for (int i = 1; i < 15; i++)
      if (i % 2 == 0)
        tmp_sum += (tmp_res[i] - '0');
    tmp_res[0] = ('0' + tmp_sum % 2);

    tmp_sum = 0;
    for (int i = 2; i < 15; i++) {
      tmp_sum += (tmp_res[i] - '0');
      if (i % 2 == 0)
        i += 2;
    }
    tmp_res[1] = ('0' + tmp_sum % 2);

    tmp_sum = 0;
    for (int i = 4; i < 15; i++)
      if (i >= 4 && i <= 6 || i >= 11 && i <= 14)
        tmp_sum += (tmp_res[i] - '0');
    tmp_res[3] = ('0' + tmp_sum % 2);

    tmp_sum = 0;
    for (int i = 8; i < 15; i++)
      tmp_sum += (tmp_res[i] - '0');
    tmp_res[7] = ('0' + tmp_sum % 2);

    res.append(tmp_res);
  }

  return res;
}

std::string crypto::hem_decode(std::string src) {
  //assuming src.size() % 15 = 0

  std::string res;
  for (int q = 0; q < (int)src.size() / 15; q++) {
    std::string tmp_src(src, q * 15, 15);

    std::string error_binary;
    int tmp_sum = 0;
    for (int i = 0; i < 15; i++)
      if (i % 2 == 0)
        tmp_sum += (tmp_src[i] - '0');
    error_binary.push_back('0' + tmp_sum % 2);

    tmp_sum = 0;
    for (int i = 1; i < 15; i++) {
      tmp_sum += (tmp_src[i] - '0');
      if (i % 2 == 0)
        i += 2;
    }
    error_binary.push_back('0' + tmp_sum % 2);

    tmp_sum = 0;
    for (int i = 3; i < 15; i++)
      if (i >= 3 && i <= 6 || i >= 11 && i <= 14)
        tmp_sum += (tmp_src[i] - '0');
    error_binary.push_back('0' + tmp_sum % 2);

    tmp_sum = 0;
    for (int i = 7; i < 15; i++)
      tmp_sum += (tmp_src[i] - '0');
    error_binary.push_back('0' + tmp_sum % 2);
    std::reverse(error_binary.begin(), error_binary.end());

    int error_position = std::stoi(base_convert(error_binary, "2", "10")) - 1;
    if (error_position > -1)
      tmp_src[error_position] = ('0' + '1' - tmp_src[error_position]);
    
    std::string tmp_res;
    for (int i = 0; i < 15; i++)
      if (i != 0 && i != 1 && i != 3 && i != 7)
        tmp_res.push_back(tmp_src[i]);

    res.append(tmp_res);
  }

  return res;
}