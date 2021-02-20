#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <clocale>
#include "crypto.h"

class CryptoFramework
{
  std::map<std::string, std::string> variables = { {"buf", ""} };
  std::map<std::string, std::string> options = { {"algorithm", ""}, {"input", "buf"}, {"output", "buf"} };

  std::string get(std::string variable_name);
  void set(std::string variable_name, std::string variable_value);

  std::string get_option(std::string option_name);
  void set_option(std::string option_name, std::string option_value);

  void run_algorithm();

  void print_variable(std::string variable_name);
  void print_option(std::string option_name);
  void print_beginning();
  void print_text(std::string text);

  void show_options();
  void show_variables();

  int process_user();

public:
  void run();
};

