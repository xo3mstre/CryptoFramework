#include "framework.h"

std::string CryptoFramework::get(std::string variable_name) {
  return user_variables[variable_name];
}

void CryptoFramework::set(std::string variable_name, std::string variable_value) {
  user_variables[variable_name] = variable_value;
}

std::string CryptoFramework::get_option(std::string option_name) {
  return options[option_name];
}

void CryptoFramework::set_option(std::string option_name, std::string option_value) {
  if (option_name == "io") {
    options["input"] = option_value;
    options["output"] = option_value;
  }
  else {
    options[option_name] = option_value;
  }
}

void CryptoFramework::show_options() {
  print_option("algorithm");
  print_option("input");
  print_option("output");
}

void CryptoFramework::run_algorithm() {
  std::string input = get(get_option("input")), algorithm = get_option("algorithm"), output;

  if (algorithm == "ascii-encode") {
    output = crypto::ascii_encode(input);
  }
  else if (algorithm == "ascii-decode") {
    output = crypto::ascii_decode(input);
  }
  else if (algorithm == "rle-encode") {
    output = crypto::rle_encode(input);
  }
  else if (algorithm == "rle-decode") {
    output = crypto::rle_decode(input);
  }
  else if (algorithm == "hem-encode") {
    output = crypto::hem_encode(input);
  }
  else if (algorithm == "hem-decode") {
    output = crypto::hem_decode(input);
  }
  else if (algorithm == "base-convert") {
    output = crypto::base_convert(input, std::stoi(get_option("bfrom")), std::stoi(get_option("bto")));
  }
  else if (algorithm == "remove-spaces") {
    output = crypto::remove_spaces(input);
  }

  set(get_option("output"), output);
}

void CryptoFramework::print_variable(std::string variable_name) {
  std::cout << variable_name << " => " << get(variable_name) << std::endl;
}

void CryptoFramework::print_option(std::string option_name) {
  if (option_name == "io") {
    print_option("input");
    print_option("output");
  }
  else {
    std::cout << option_name << " => " << options[option_name] << std::endl;
  }
}

void CryptoFramework::print_beginning() {
  std::cout << "Crypto-CLI";
  if (options["algorithm"] != "")
    std::cout << "[" << options["algorithm"] << "]";
  std::cout << "> ";
}

void CryptoFramework::print_text(std::string text) {
  std::cout << text << std::endl;
}

void CryptoFramework::run() {
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);

  while (process_user() != 1) {}
}

int CryptoFramework::process_user() {
  print_beginning();

  std::string user_input;
  std::getline(std::cin, user_input);
  std::stringstream user_words(user_input);

  std::string command;
  user_words >> command;
  if (command == "GET") {
    std::string variable_name;
    user_words >> variable_name;
    print_variable(variable_name);

    return 0;
  }
  else if (command == "SET") {
    std::string variable_name, variable_value, tmp_value;

    user_words >> variable_name;
    while (user_words >> tmp_value)
      variable_value.append(tmp_value);

    set(variable_name, variable_value);

    print_variable(variable_name);
    return 0;
  }
  else if (command == "USE") {
    std::string option_name, option_value;
    user_words >> option_name >> option_value;
    set_option(option_name, option_value);
    print_option(option_name);

    return 0;
  }
  else if (command == "OPTIONS") {
    show_options();

    return 0;
  }
  else if (command == "RUN") {
    run_algorithm();
    print_variable(get_option("output"));
    return 0;
  }
  else if (command == "CLEAR") {
    system("cls");

    return 0;
  }
  else if (command == "EXIT") {
    print_text("Goodbye.");

    return 1;
  }
  else {
    print_text("Unknown command.");

    return 2;
  }
}