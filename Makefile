CXXFLAGS = -O3 -Wall -Wextra -Werror -std=c++23 -pedantic -pedantic-errors
ifeq ($(DEBUG), 1)
	CXXFLAGS = -O0 -g --std=c++23
endif
CHECKFLAGS = --enable=all --std=c++23 --error-exitcode=1 --check-level=exhaustive --suppress=missingIncludeSystem --suppress=checkersReport --suppress=unusedFunction --suppress=unmatchedSuppression --inline-suppr
TIDYFLAGS = -checks=bugprone-*,cert-*,cppcoreguidelines-*,hicpp-*,misc-*,-misc-use-internal-linkage,-misc-non-private-member-variables-in-classes,-misc-no-recursion,modernize-*,performance-*,portability-*,readability-*,-readability-identifier-length  --warnings-as-errors=*

# Work around the clang-tidy bug
HACKFLAGS = --sysroot=/ -nostdinc++  -isystem /opt/homebrew/include -isystem /opt/homebrew/opt/gcc/include/c++/15 -isystem /opt/homebrew/opt/gcc/include/c++/15/aarch64-apple-darwin24 -nostdlib 

CXX = clang++

SRC = $(shell find src -type f -name '*.cpp')
OBJS = $(SRC:.cpp=.o)

aoc2025: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/main.o: src/main.cpp src/days.hpp src/common.hpp
	cppcheck $(CHECKFLAGS) $<
	clang-tidy $(TIDYFLAGS) $< -- $(CXXFLAGS) $(HACKFLAGS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/%.o: src/%.cpp src/common.hpp
	cppcheck $(CHECKFLAGS) $<
	clang-tidy $(TIDYFLAGS) $< -- $(CXXFLAGS) $(HACKFLAGS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


src/days.hpp: $(shell find src/ -type f -name 'day*.cpp')
	echo "#pragma once" > $@
	echo '#include "common.hpp"' >> $@
	for n in $(shell find src/ -name day*.cpp | gsed 's|src/day\([0-9]\+\).cpp|\1|'); do echo "DAY($$n);" >> $@; done


clean:
	rm -f src/days.hpp
	rm -f src/*.o
	rm -f aoc2025
