#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <string>

namespace Komodo {

class Expander {
public:
  static void expand(const std::string &file_basename,
                     std::ostream &out = std::cout) {
    std::string source_path = "src/code/" + file_basename + ".cpp";
    if (!std::filesystem::exists(source_path)) {
      // Try without prefix if user gave full path or just generic check
      if (std::filesystem::exists(file_basename + ".cpp")) {
        source_path = file_basename + ".cpp";
      } else if (std::filesystem::exists(file_basename)) {
        source_path = file_basename;
      } else {
        throw std::runtime_error("File not found: " + source_path);
      }
    }

    std::set<std::string> included_files;
    process_file(source_path, out, included_files);
  }

private:
  static void process_file(const std::string &path, std::ostream &out,
                           std::set<std::string> &included_files) {
    std::ifstream file(path);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file: " + path);
    }

    std::string line;
    static const std::regex include_library_regex(
        R"(^\s*#include\s*\"(library/.*)\"\s*$)");
    std::smatch match;

    while (std::getline(file, line)) {
      // Strip #pragma once
      if (line.find("#pragma once") != std::string::npos) {
        continue;
      }

      if (std::regex_match(line, match, include_library_regex)) {
        std::string library_path = "src/" + match[1].str();
        if (included_files.find(library_path) == included_files.end()) {
          included_files.insert(library_path);
          process_file(library_path, out, included_files);
        }
      } else {
        out << line << "\n";
      }
    }
  }
};

} // namespace Komodo
