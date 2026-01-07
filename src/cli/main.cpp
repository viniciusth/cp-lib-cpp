#include <iostream>
#include <string>
#include <string_view>

#include "expand.hpp"
#include "runner.hpp"

void print_usage() {
  std::cout << "Usage: komodo <command> [args]\n";
  std::cout << "Commands:\n";
  std::cout << "  run <file>    Compile and run a solution (default a)\n";
  std::cout
      << "  expand <file> Expand a solution into a single file (default a)\n";
  std::cout << "  stress <file> Stress test a solution (default a)\n";
}

int main(int argc, char **argv) {
  if (argc < 2) {
    print_usage();
    return 1;
  }

  std::string_view command = argv[1];
  std::string file = "a"; // Default file
  if (argc >= 3) {
    file = argv[2];
  }

  try {
    if (command == "run") {
      Komodo::Runner::run(file);
    } else if (command == "expand") {
      Komodo::Expander::expand(file);
    } else if (command == "stress") {
      Komodo::Runner::stress(file);
    } else {
      std::cerr << "Unknown command: " << command << "\n";
      print_usage();
      return 1;
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
