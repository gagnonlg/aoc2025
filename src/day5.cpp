#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "common.hpp"

namespace day5 {

auto parse_input(const std::string& input)
    -> std::pair<std::vector<std::pair<size_t, size_t>>,
		 std::vector<size_t>> {
    std::vector<std::string> lines = split_at(input, '\n', true);

    std::vector<std::pair<size_t, size_t>> ranges;
    std::vector<size_t> ingredients;
    
    std::vector<std::string>::size_type i = 0; // NOLINT(readability-identifier-length)
    for (; i < lines.size(); i++) {
	if (lines.at(i).empty()) {
	    i++;
	    break;
	}
	const std::vector<std::string> fields = split_at(lines.at(i), '-');
	ranges.emplace_back(std::stoull(fields.at(0)), std::stoull(fields.at(1)));
    }
    
    for (; i < lines.size(); i++) {
	ingredients.push_back(std::stoull(lines.at(i)));
    }

    std::ranges::for_each(ranges, [](const auto& pair)->void{CHECK(pair.first <= pair.second);});

    return std::make_pair(ranges, ingredients);
}

auto is_fresh(size_t ingredient_id,
	      const std::vector<std::pair<size_t, size_t>>& ranges) -> bool {
    return std::ranges::any_of(
	ranges,
	[&] (const auto& rng) -> bool {
	    auto& [start, end] = rng;
	    return (ingredient_id >= start && ingredient_id <= end);
	});
}

auto part_1(const std::string& input) -> Answer
{
    auto [ranges, ingredients] = parse_input(input);
    return std::ranges::count_if(
	ingredients,
	[&] (size_t iid) -> bool {
	    return is_fresh(iid, ranges);
	});
}

auto ranges_overlap(const std::pair<size_t, size_t>& lhs,
		    const std::pair<size_t, size_t>& rhs) -> bool {
    return rhs.first <= lhs.second;
}

auto deoverlap(std::vector<std::pair<size_t, size_t>>& ranges) -> void { 

    if (ranges.size() <= 1) {
	return;
    }
    std::ranges::sort(ranges);
    auto iter = ranges.begin();
    // cppcheck-suppress invalidContainer
    while (iter < ranges.end() - 1) {
	auto iter2 = std::next(iter);
	if (ranges_overlap(*iter, *iter2)) {
	    iter->second = std::max(iter->second, iter2->second);
	    ranges.erase(iter2);
	} else {
	    iter = std::next(iter);
	}
    }
    
}
    
auto part_2(const std::string& input) -> Answer
{
    auto [ranges, ingredients] = parse_input(input);
    UNUSED(ingredients);
    deoverlap(ranges);
    return std::accumulate(ranges.begin(), ranges.end(), 0LL,
			   [](Answer acc, const auto& rng) -> Answer {
			       return acc + rng.second - rng.first + 1;
			   });
}

void tests() // NOLINT(readability-function-cognitive-complexity)
{
    const std::string test_input = 
	"3-5\n"
	"10-14\n"
	"16-20\n"
	"12-18\n"
	"\n"
	"1\n"
	"5\n"
	"8\n"
	"11\n"
	"17\n"
	"32\n";

    auto [ranges, ingredients] = parse_input(test_input);
    CHECK(ranges.size() == 4);
    CHECK(ranges[0].first == 3);
    CHECK(ranges[0].second == 5);
    CHECK(ingredients == std::vector<size_t>({1,5,8,11,17,32}));

    CHECK(!is_fresh(1, ranges));
    CHECK(is_fresh(5, ranges));
    CHECK(!is_fresh(8, ranges));
    CHECK(is_fresh(11, ranges));
    CHECK(is_fresh(17, ranges));
    CHECK(!is_fresh(32, ranges));

    CHECK(part_1(test_input) == 3);

    CHECK(ranges_overlap(std::make_pair(10, 14), std::make_pair(12, 18)));
    CHECK(ranges_overlap(std::make_pair(10, 18), std::make_pair(16, 20)));

    deoverlap(ranges);
    CHECK(ranges.size() == 2);
    CHECK(ranges[0].first == 3 && ranges[0].second == 5); // NOLINT(readability-simplify-boolean-expr)
    CHECK(ranges[1].first == 10 && ranges[1].second == 20); // NOLINT(readability-simplify-boolean-expr)

    std::vector<std::pair<size_t, size_t>> test_ranges = {{1,10}, {3,5}, {2,11}}; // NOLINT(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)
    deoverlap(test_ranges);
    CHECK(test_ranges.size() == 1);
    CHECK(test_ranges[0].first == 1 && test_ranges[0].second == 11); // NOLINT(readability-simplify-boolean-expr)

    
    CHECK(part_2(test_input) == 14);

    // 334572241531681 too low
    // 320578958725143
    // 334572241531681
}

     
} // namespace day5

