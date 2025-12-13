#include <cstddef>
// #include <functional>
#include <string>
#include <utility>
#include <vector>

#include "common.hpp"

namespace day4 {

constexpr char PAPER_SYMBOL = '@';
constexpr char EMPTY_SYMBOL = '.';

template <typename T>
auto reachable_indices(const Grid<T>& grid, size_t i, size_t j) // NOLINT
    -> std::vector<std::pair<size_t, size_t>> {
    std::vector<std::pair<size_t, size_t>> idx;
    const bool firsti = i == 0;
    const bool firstj = j == 0;
    const bool lasti = i == grid.nrows - 1;
    const bool lastj = j == grid.ncols - 1;

    // i,   j+1
    if (!lastj) {
	idx.emplace_back(i, j+1);
    }
    // i-1, j+1
    if (!firsti && !lastj) {
	idx.emplace_back(i-1, j+1);
    }
    // i-1, j
    if (!firsti) {
	idx.emplace_back(i-1, j);
    }
    // i-1, j-1,
    if (!firsti && !firstj) {
	idx.emplace_back(i-1, j-1);
    }
    // i,   j-1,
    if (!firstj) {
	idx.emplace_back(i, j-1);
    }
    // i+1, j-1,
    if (!lasti && !firstj) {
	idx.emplace_back(i+1, j-1);
    }
    // i+1, j,
    if (!lasti) {
	idx.emplace_back(i+1, j);
		}
    // i+1, j+1
    if (!lasti && !lastj) {
	idx.emplace_back(i+1, j+1);
    }

    return idx;
}

auto is_accessible_roll(const Grid<char>& grid, size_t i, size_t j) -> bool // NOLINT
{
    if (grid.at(i, j) != PAPER_SYMBOL) {
	return false;
    }

    size_t count = 0;
    for (const auto& idx : reachable_indices(grid, i, j)) {
	if (grid.at(idx.first, idx.second) == PAPER_SYMBOL) {
	    if (++count == 4) {
		return false;
	    }
	}
    }

    return true;
}

auto part_1(const std::string& input) -> Answer
{
    const Grid<std::string::value_type> grid(
	input,
	[](auto, auto, char arg3) -> char {return arg3;}
	);

    Answer counter = 0;

    for (size_t i = 0; i < grid.nrows; i++) {
	for (size_t j = 0; j < grid.ncols; j++) {
	    if (is_accessible_roll(grid, i, j)) {
		counter++;
	    }
	}
    }
    
    return counter;
}

auto try_remove_roll(Grid<char>& grid, size_t i, size_t j) -> Answer { // NOLINT
    if (grid.at(i, j) != PAPER_SYMBOL) {
	return 0;
    }

    Answer counter{0};
    
    if (is_accessible_roll(grid, i, j)) {
	grid.ref(i, j) = EMPTY_SYMBOL;
	counter++;

	// for (const auto& idx : reachable_indices(grid, i, j)) {
	//     if (idx.first < i || idx.second < j) {
	// 	counter = try_remove_roll(grid, idx.first, idx.second);
	//     }
	// }
    }

    return counter;
}

auto part_2(const std::string& input) -> Answer
{
    Grid<std::string::value_type> grid(
	input,
	[](auto, auto, char arg3) -> char {return arg3;}
	);

    Answer counter = 0;
    Answer update = 0;

    do { // NOLINT(cppcoreguidelines-avoid-do-while)
	update = 0;
	for (size_t i = 0; i < grid.nrows; i++) {
	    for (size_t j = 0; j < grid.ncols; j++) {
		update += try_remove_roll(grid, i, j);
	    }
	}
	counter += update;
    } while (update != 0);
    
    return counter;
}

void tests() // NOLINT(readability-function-cognitive-complexity)
{
    const std::string test_input =
	"..@@.@@@@.\n"
	"@@@.@.@.@@\n"
	"@@@@@.@.@@\n"
	"@.@@@@..@.\n"
	"@@.@@@@.@@\n"
	".@@@@@@@.@\n"
	".@.@.@.@@@\n"
	"@.@@@.@@@@\n"
	".@@@@@@@@.\n"
	"@.@.@@@.@.\n";

    const Grid<std::string::value_type> grid(
	test_input,
	[](auto, auto, char arg3) -> char {return arg3;}
	);

    CHECK(is_accessible_roll(grid, 0, 2));
    CHECK(!is_accessible_roll(grid, 0, 0));
    CHECK(!is_accessible_roll(grid, 1, 1));
    
    CHECK(part_1(test_input) == 13);

    CHECK(part_2(test_input) == 43);


    
}

     
} // namespace day4

