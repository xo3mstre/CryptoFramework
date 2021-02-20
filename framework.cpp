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
  else if (option_name == "algo")
    options["algorithm"] = option_value;
  else 
    options[option_name] = option_value;
}

void CryptoFramework::show_options() {
  print_option("algorithm");
  print_option("input");
  print_option("output");

  bool is_additional_options_exists = false;
  for (std::pair<std::string, std::string> opt : options)
    if (opt.first != "algorithm" && opt.first != "input" && opt.first != "output") {
      if (!is_additional_options_exists) {
        is_additional_options_exists = true;
        print_text("--------------------");
      }

      print_option(opt.first);
    }
}

void CryptoFramework::run_algorithm() {
  std::string input = get(get_option("input")), algorithm = get_option("algorithm"), output;

  if (algorithm == "ascii-encode") {
    output = crypto::ascii_encode(input);
  }
  else if (algorithm == "ascii-decode") {
    output = crypto::ascii_decode(input);
  }
  else if (algorithm == "base64-encode") {
    output = crypto::base64_encode(input);
  }
  else if (algorithm == "base64-decode") {
    output = crypto::base64_decode(input);
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
    output = crypto::base_convert(input, get_option("bfrom"), get_option("bto"), get_option("len"));
  }
  else if (algorithm == "remove-spaces") {
    output = crypto::remove_spaces(input);
  }
  else if (algorithm == "add-spaces") {
    output = crypto::add_spaces(input, get_option("len"));
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
  else if (option_name == "algo")
    print_option("algorithm");
  else
    std::cout << option_name << " => " << options[option_name];

  if (option_name == "input" || option_name == "output")
    std::cout << " => " << get(get_option(option_name));

  std::cout << std::endl;
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
  if (command == "HELP" || command == "?") {
      print_text(
        "Cryptografic Framework lets you interact with cryptographic algorithms using simple command-line interface.\n"
        "\n"
        "Availible commands (case-sensitive):\n"
        "-------------------------------------------------------------\n"
        "|  GET <variable>           |  Get value of the variable    |\n"
        "|  SET <variable> <value>   |  Set value of the variable    |\n"
        "|  USE <option> <value>     |  Set value of the option      |\n"
        "|  OPTIONS                  |  List options                 |\n"
        "|  RUN                      |  Run choosen algorithm        |\n"
        "|  CLEAR                    |  Clear output (cls)           |\n"
        "|  EXIT                     |  Exit program                 |\n"
        "|  HELP / ?                 |  Print this help message      |\n"
        "-------------------------------------------------------------\n"
        "\n"
        "There are two types of stored data: variables and options.\n"
        "Semantic load is stored in variables. Other data for algorithms is stored in options.\n"
        "There are three default options required for every algorithm: \n"
        "-------------------------------------------------------------\n"
        "|  algorithm                |  Choosen algorithm            |\n"
        "|  input (default: 'buf')   |  Name of the input variable   |\n"
        "|  output (default: 'buf')  |  Name of the output variable  |\n"
        "-------------------------------------------------------------\n"
        "Some algorithms may require additional options. In this case you need to define these options using USE command.\n"
        "(As example, 'base-convert' algorithm requires two additional options: 'bfrom' and 'bto' - base of the input and base of the output)\n"
        "\n"
        "As default, algorithms input and output are a bit sequence. There are some exceptions like 'ascii-encode/decode' or 'base64-encode/decode'.\n"
        "Be careful: SET command ignores spaces. If it is important, you can add them using 'add-spaces' algorithm.\n"
      );
  }
  else if (command == "GET") {
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