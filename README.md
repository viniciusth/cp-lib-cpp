# cp-lib-cpp

A competitive programming environment in C++, mirrored after `cp-lib-rs`. This project provides a CLI tool `komodo` to manage solution files, library dependencies, and stress testing.

## Dependencies

- **C++ Compiler**: `g++` or `clang++` with support for **C++23**.
  - Verified with GCC 13.3.
- **Micro-dependencies**: The CLI tool uses standard library components (`<filesystem>`, `<iostream>`, etc.). No external package managers (like vcpkg/conan) are required for the base setup.

## Setup

1. **Compile the CLI tool**:
   You can use the provided `Makefile` to automatically build the tool and run commands.
   ```bash
   make
   ```
   Or manually:
   ```bash
   mkdir -p bin
   g++ -std=c++23 -O2 src/cli/main.cpp -I src/cli -I src -o bin/komodo
   ```

2. **(Optional) Add to PATH**:
   You can add the `bin` directory to your PATH or symlink `bin/komodo` to a global specific location to run it from anywhere.

## Usage

For the most convenient experience, use the included `./k` script. It automatically builds the project if needed and accepts positional arguments.

### Run a Solution
```bash
make run FILE=a
```

### Expand/Bundle
```bash
make expand FILE=a
```

### Stress Testing
```bash
make stress FILE=a
```

This expects three files to exist:
1. `src/code/a.cpp` (Your optimized solution)
2. `src/code/brute.cpp` (Brute force/Reference solution)
3. `src/code/gen.cpp` (Test case generator)

It runs in a loop:
1. `gen > input.txt`
2. `brute < input.txt > expected.txt`
3. `a < input.txt > actual.txt`
4. Compares `expected.txt` and `actual.txt`. Stops if they differ.

## Project Structure

```text
cp-lib-cpp/
├── src/
│   ├── cli/        # Source code for the komodo tool
│   ├── library/    # Reusable CP library headers
│   └── code/       # Your solution files (a.cpp, b.cpp, etc.)
├── bin/            # Output directory for the compiled CLI
└── README.md
```
