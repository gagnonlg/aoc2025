#include <algorithm>
#include <cmath>
#include <iostream> // NOLINT(misc-include-cleaner)
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "common.hpp"

namespace day1 {

constexpr int INITIAL_DIAL = 50;
constexpr int MAX_DIAL = 100;


auto remove_full_turns(int& rot) -> int {
    // Start by counting full rotations
    const int turns = std::abs(rot) / MAX_DIAL;
    if (rot > 0) {
	rot -= turns * MAX_DIAL;
    }
    if (rot < 0) {
	rot += turns * MAX_DIAL;
    }
    return turns;
}

auto rotate(int dial, int rot) -> int
{
    (void)remove_full_turns(rot);
    int dst = dial + rot;
    if (dst < 0) {
	dst = MAX_DIAL + dst;
    }
    return dst % MAX_DIAL;
}

auto parserot(const std::string& rotstr) -> int
{
    int sign = 0;
    switch (rotstr.at(0)) {
    case 'L':
	sign = -1;
	break;
    case 'R':
	sign = +1;
	break;
    default:
	throw std::runtime_error("invalid rot str");
    }

    const int rot = std::stoi(rotstr.substr(1));

    return sign * rot;
}

auto getpass(const std::vector<int>& moves, int dial = INITIAL_DIAL) -> int
{
    auto acc = std::accumulate(
	moves.begin(), moves.end(), std::make_pair(dial, 0),
	[](std::pair<int, int> acc, int move) -> std::pair<int, int> {
	    acc.second += ((acc.first = rotate(acc.first, move)) == 0);
	    return acc;
	});    
    return acc.second;
}


auto getmoves(const std::string& input) -> std::vector<int> 
{
    std::vector<int> moves;
    std::vector<std::string> smoves = split_lines(input);
    std::ranges::transform(smoves, std::back_inserter(moves), parserot);
    return moves;
}

auto part_1(const std::string& input) -> Answer
{
    return getpass(getmoves(input));
}

auto crosses_zero(int dial, int rot) -> Answer
{
    int crossings = remove_full_turns(rot);

    if (rot == 0 || dial == 0) {
	return crossings;
    }

    // Now Compute distance from dial to 0 in direction of rotation
    const int dist0 = rot > 0 ? MAX_DIAL - dial : dial;

    // if rotation is qeg to that distance, there was either a crossing or we ended on 0
    // don't count if we starts on 0
    if (std::abs(rot) >= dist0) {
	crossings++;
    }

    return crossings;
}

auto getpass_0x434C49434B(const std::vector<int>& moves) -> Answer
{
    auto acc = std::accumulate(
	moves.begin(), moves.end(), std::make_pair(INITIAL_DIAL, 0),
	[](std::pair<int, Answer> acc, int move) -> std::pair<int, Answer> {
	    acc.second += crosses_zero(acc.first, move);
	    acc.first = rotate(acc.first, move);
	    return acc;
	});    
    return acc.second;
}


auto part_2(const std::string& input) -> Answer
{
  return getpass_0x434C49434B(getmoves(input));
}

void tests() // NOLINT(readability-function-cognitive-complexity)
{
    // NOLINTBEGIN(bugprone-assignment-in-if-condition)

    int dial = 11; // NOLINT
    CHECK((dial = rotate(dial, 8)) == 19);
    CHECK((dial = rotate(dial, -19)) == 0);

    CHECK(rotate(0, -1) == 99);
    CHECK(rotate(99, 1) == 0);

    dial = 5; // NOLINT
    CHECK((dial = rotate(dial, -10)) == 95);
    CHECK((dial = rotate(dial, 5)) == 0);

    //////////////////////////////////////////////

    CHECK(parserot("L68") == -68);
    CHECK(parserot("R48") == +48);

    //////////////////////////////////////////////

    const std::string test_input_1 = 
	"L68\n"
	"L30\n"
	"R48\n"
	"L5\n"
	"R60\n"
	"L55\n"
	"L1\n"
	"L99\n"
	"R14\n"
	"L82\n";
    
    std::vector<std::string> moves = split_lines(test_input_1);

    dial = INITIAL_DIAL;
    CHECK((dial = rotate(dial, parserot(moves.at(0)))) == 82);
    CHECK((dial = rotate(dial, parserot(moves.at(1)))) == 52);
    CHECK((dial = rotate(dial, parserot(moves.at(2)))) == 0);
    CHECK((dial = rotate(dial, parserot(moves.at(3)))) == 95);
    CHECK((dial = rotate(dial, parserot(moves.at(4)))) == 55);
    CHECK((dial = rotate(dial, parserot(moves.at(5)))) == 0);
    CHECK((dial = rotate(dial, parserot(moves.at(6)))) == 99);
    CHECK((dial = rotate(dial, parserot(moves.at(7)))) == 0);
    CHECK((dial = rotate(dial, parserot(moves.at(8)))) == 14);
    CHECK((dial = rotate(dial, parserot(moves.at(9)))) == 32);
    (void)dial;

    std::vector<int> imoves;
    std::ranges::transform(moves, std::back_inserter(imoves), parserot);

    CHECK(getpass(imoves) == 3);

    CHECK(part_1(test_input_1) == 3);

    //////////////////////////////////////////////

    dial = INITIAL_DIAL;
    CHECK(crosses_zero(dial, parserot(moves.at(0))));
    dial = rotate(dial, parserot(moves.at(0)));
    CHECK(!crosses_zero(dial, parserot(moves.at(1))));
    dial = rotate(dial, parserot(moves.at(1)));
    CHECK(crosses_zero(dial, parserot(moves.at(2))));
    dial = rotate(dial, parserot(moves.at(2)));
    CHECK(!crosses_zero(dial, parserot(moves.at(3))));
    dial = rotate(dial, parserot(moves.at(3)));
    CHECK(crosses_zero(dial, parserot(moves.at(4))));
    dial = rotate(dial, parserot(moves.at(4)));
    CHECK(crosses_zero(dial, parserot(moves.at(5))));
    dial = rotate(dial, parserot(moves.at(5))); // NOLINT
    CHECK(!crosses_zero(dial, parserot(moves.at(6)))); 
    dial = rotate(dial, parserot(moves.at(6))); // NOLINT
    CHECK(crosses_zero(dial, parserot(moves.at(7))));
    dial = rotate(dial, parserot(moves.at(7))); // NOLINT
    CHECK(!crosses_zero(dial, parserot(moves.at(8))));
    dial = rotate(dial, parserot(moves.at(8))); // NOLINT
    CHECK(crosses_zero(dial, parserot(moves.at(9))));

    CHECK(getpass_0x434C49434B(imoves) == 6);


    dial = INITIAL_DIAL;
    CHECK(crosses_zero(dial, +200) == 2);
    CHECK(crosses_zero(dial, -200) == 2);
    dial = 0;
    CHECK(crosses_zero(dial, +100) == 1);
    CHECK(crosses_zero(dial, -100) == 1);
    dial = INITIAL_DIAL;
    CHECK(crosses_zero(dial, MAX_DIAL + (MAX_DIAL - INITIAL_DIAL)) == 2);

    dial = INITIAL_DIAL;
    CHECK(crosses_zero(dial, 1000) == 10);

    dial = 0;
    CHECK(rotate(dial, -377) == (MAX_DIAL - 77));
	
    
    // NOLINTEND(bugprone-assignment-in-if-condition)
    
}

     
} // namespace day1
