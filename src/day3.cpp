#include <algorithm>
// #include <cstddef>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "common.hpp"

namespace day3 {

auto maximum_joltage(const std::string& bank) -> Answer {
    auto itl = std::ranges::max_element(bank.begin(), std::prev(bank.end()));
    auto itr = std::ranges::max_element(std::next(itl), bank.end());
    const std::string jolt({*itl, *itr});
    return std::stoll(jolt);
}

auto overmaximum_joltage(const std::string& bank) -> Answer {
    const int nmax = 12;
    std::string chosen;

    CHECK(bank.size() >= nmax);

    auto start = bank.begin();
    for (int i = 0; i < nmax; i++) {
	auto end = bank.end() - nmax + i + 1;
	CHECK(end > start);
	auto pick = std::ranges::max_element(start, end);
	chosen.push_back(*pick);
	start = std::next(pick);
    }

    return std::stoll(chosen);
}

auto part_1(const std::string& input) -> Answer
{
    std::vector<std::string> lines = split_lines(input);
    std::vector<Answer> joltages;
    std::ranges::transform(lines, std::back_inserter(joltages), maximum_joltage);
    return std::accumulate(joltages.begin(), joltages.end(), 0LL);
}

auto part_2(const std::string& input) -> Answer
{
    std::vector<std::string> lines = split_lines(input);
    std::vector<Answer> joltages;
    std::ranges::transform(lines, std::back_inserter(joltages), overmaximum_joltage);
    return std::accumulate(joltages.begin(), joltages.end(), 0LL);
}

void tests() // NOLINT(readability-function-cognitive-complexity)
{
    const std::string test_input_1 =
	"987654321111111\n"
	"811111111111119\n"
	"234234234234278\n"
	"818181911112111\n";

    const std::vector<std::string> test_lines_1 = split_lines(test_input_1);

    CHECK(maximum_joltage(test_lines_1.at(0)) == 98);
    CHECK(maximum_joltage(test_lines_1.at(1)) == 89);
    CHECK(maximum_joltage(test_lines_1.at(2)) == 78);
    CHECK(maximum_joltage(test_lines_1.at(3)) == 92);

    CHECK(part_1(test_input_1) == 357);

    CHECK(overmaximum_joltage(test_lines_1.at(0)) == 987654321111);
    CHECK(overmaximum_joltage(test_lines_1.at(1)) == 811111111119);
    CHECK(overmaximum_joltage(test_lines_1.at(2)) == 434234234278);
    CHECK(overmaximum_joltage(test_lines_1.at(3)) == 888911112111);

    CHECK(part_2(test_input_1) == 3121910778619);
    
}

     
} // namespace day3

