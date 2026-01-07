#pragma once

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace Komodo {

class Runner {
public:
  static void run(const std::string &file_basename) {
    std::string source = get_source_path(file_basename);
    std::string binary = "build/" + file_basename;

    compile(source, binary);

    std::cout << "Running " << binary << "...\n";
    int ret = std::system(binary.c_str());
    if (ret != 0) {
      throw std::runtime_error("Execution failed");
    }
  }

  static void stress(const std::string &file_basename) {
    std::string sol_src = get_source_path(file_basename);
    std::string brute_src = "src/code/brute.cpp";
    std::string gen_src = "src/code/gen.cpp";

    std::string sol_bin = "build/" + file_basename;
    std::string brute_bin = "build/brute";
    std::string gen_bin = "build/gen";

    std::cout << "Compiling solution...\n";
    compile(sol_src, sol_bin);
    std::cout << "Compiling brute...\n";
    compile(brute_src, brute_bin);
    std::cout << "Compiling gen...\n";
    compile(gen_src, gen_bin);

    int test_case = 1;
    while (true) {
      // 1. Generate Input
      std::string gen_cmd = gen_bin + " > input.txt";
      if (std::system(gen_cmd.c_str()) != 0)
        throw std::runtime_error("Generator failed");

      // 2. Run Brute
      std::string brute_cmd = brute_bin + " < input.txt > expected.txt";
      if (std::system(brute_cmd.c_str()) != 0)
        throw std::runtime_error("Brute failed");

      // 3. Run Solution
      std::string sol_cmd = sol_bin + " < input.txt > actual.txt";
      if (std::system(sol_cmd.c_str()) != 0)
        throw std::runtime_error("Solution failed");

      // 4. Compare
      if (files_differ("expected.txt", "actual.txt")) {
        std::cerr << "Mismatch found on test case " << test_case << "!\n";
        std::cerr << "Input:\n";
        print_file("input.txt");
        std::cerr << "Expected:\n";
        print_file("expected.txt");
        std::cerr << "Actual:\n";
        print_file("actual.txt");
        break;
      }

      std::cout << "Test Case " << test_case++ << " Passed\n";
    }
  }

private:
  static std::string get_source_path(const std::string &basename) {
    // Simple check
    if (std::filesystem::exists("src/code/" + basename + ".cpp")) {
      return "src/code/" + basename + ".cpp";
    }
    return basename;
  }

  static void compile(const std::string &source, const std::string &output) {
    // Enforce C++23, -O2, -DLOCAL
    // Use string concatenation instead of std::format
    std::string cmd = "g++ -std=c++23 -O2 -DLOCAL -include "
                      "src/library/bits/stdc++.h -fuse-ld=gold -I src " +
                      source + " -o " + output;
    std::cout << "CMD: " << cmd << "\n";
    int ret = std::system(cmd.c_str());
    if (ret != 0) {
      throw std::runtime_error("Compilation failed for " + source);
    }
  }

  static bool files_differ(const std::string &file1, const std::string &file2) {
    std::ifstream f1(file1), f2(file2);
    std::string c1((std::istreambuf_iterator<char>(f1)),
                   std::istreambuf_iterator<char>());
    std::string c2((std::istreambuf_iterator<char>(f2)),
                   std::istreambuf_iterator<char>());

    return c1 != c2;
  }

  static void print_file(const std::string &path) {
    std::ifstream f(path);
    std::cout << f.rdbuf() << std::endl;
  }
};

} // namespace Komodo
