CXX = g++
CXXFLAGS = -std=c++23 -O2 -DLOCAL
CLI_BIN = bin/komodo
SRC_DIR = src

# CLI Sources (approximation for dependency tracking)
CLI_SRCS = $(shell find src/cli -name "*.cpp" -o -name "*.hpp")

# Default file to operate on
FILE ?= a

.PHONY: all run expand stress clean cmake

cmake:
	cmake -S . -B build
	ln -sf build/compile_commands.json compile_commands.json

pch:
	$(CXX) $(CXXFLAGS) -x c++-header src/library/bits/stdc++.h -o src/library/bits/stdc++.h.gch

all: $(CLI_BIN)

# Optimization Flags
FAST_FLAGS = -include src/library/bits/stdc++.h -fuse-ld=gold

$(CLI_BIN): $(CLI_SRCS)
	@mkdir -p bin
	@echo "Building Komodo CLI..."
	$(CXX) $(CXXFLAGS) $(FAST_FLAGS) src/cli/main.cpp -I src/cli -I src -o $(CLI_BIN)

run: $(CLI_BIN)
	@mkdir -p build
	@$(CLI_BIN) run $(FILE)

expand: $(CLI_BIN)
	@$(CLI_BIN) expand $(FILE)

stress: $(CLI_BIN)
	@mkdir -p build
	@$(CLI_BIN) stress $(FILE)

test:
	@mkdir -p bin/tests
	$(CXX) $(CXXFLAGS) $(FAST_FLAGS) tests/stress_segtree.cpp -I src -o bin/tests/stress_segtree
	@./bin/tests/stress_segtree
	$(CXX) $(CXXFLAGS) $(FAST_FLAGS) tests/test_persistent_segtree.cpp -I src -o bin/tests/test_persistent_segtree
	@./bin/tests/test_persistent_segtree

clean:
	rm -rf bin build input.txt expected.txt actual.txt src/library/bits/stdc++.h.gch
