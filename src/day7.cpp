#include <algorithm>
#include <cstdint>
#include <iostream> // NOLINT(misc-include-cleaner)
#include <iterator>
#include <map>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "common.hpp"

namespace day7 {

constexpr char SOURCE_SYMBOL = 'S';
constexpr char SPLITTER_SYMBOL = '^';

struct Splitters {
    using Pos = std::pair<int64_t, int64_t>;
    std::map<Pos, std::vector<Pos>> map;
    std::map<Pos, Answer> counts;
    std::pair<int64_t, int64_t> limits;
    std::pair<int64_t, int64_t> source;

    auto mark_activations() -> void;
    auto mark_activations(std::pair<int64_t, int64_t> from, std::pair<int64_t, int64_t> parent) -> void;

    auto count_activations() -> void;
};

auto parse_input(const std::string &input) -> Splitters {
    Splitters splitters;
    std::vector<std::string> lines = split_lines(input);
    splitters.limits.first = static_cast<int64_t>(lines.size());
    splitters.limits.second = static_cast<int64_t>(lines.front().size());
    for (auto i_it = lines.begin(); i_it != lines.end(); i_it++) {
	const int64_t i_pos = std::distance(lines.begin(), i_it);
        for (auto j_it = i_it->begin(); j_it != i_it->end(); j_it++) {
            const int64_t j_pos = std::distance(i_it->begin(), j_it);
            if (*j_it == SOURCE_SYMBOL) {
		splitters.source = {i_pos, j_pos};
            } else if (*j_it == SPLITTER_SYMBOL) {
		splitters.map.insert({{i_pos, j_pos}, {}});
	    }
        }
    }
    return splitters;
}


auto Splitters::mark_activations(std::pair<int64_t, int64_t> from, std::pair<int64_t, int64_t> parent) -> void {

    // Terminal cases
    if (from.first >= limits.first || from.second >= limits.second || from.first < 0 || from.second < 0) {
        return;
    }
    
    if (map.contains({from.first + 1, from.second})) {
	const bool first_visit = map.at({from.first + 1, from.second}).empty();
	map.at({from.first + 1, from.second}).push_back(parent);
        if (first_visit) {
	    mark_activations({from.first + 1, from.second - 1}, {from.first + 1, from.second});
            mark_activations({from.first + 1, from.second + 1}, {from.first + 1, from.second});
	}
    } else {
	mark_activations({from.first + 1, from.second}, parent);
    }
}

auto Splitters::mark_activations() -> void {
    mark_activations(source, source);
}


auto part_1(const std::string &input) -> Answer {
    Splitters splitters = parse_input(input);
    splitters.mark_activations();
    return std::ranges::count_if(splitters.map, [](const auto& x)->bool{return !x.second.empty();});
}

auto Splitters::count_activations() -> void {
    for (int64_t rank = 0; rank < limits.first; rank++) {
	std::ranges::for_each(map, [&](const auto& pair) -> void {
	    auto& [pos, parents] = pair;
	    if (parents.empty()) {
		counts[pos] = 0;
	    } else {
		counts[pos] = std::accumulate(parents.begin(), parents.end(), 0LL,
					      [&](Answer acc, const auto &ppos) -> Answer {
						  if (ppos == source) {
						      return 1;
						  }
						  return acc + counts.at(ppos);
					      });
	    }
	});
    }
}

template <typename T> auto printv(const std::vector<T>& v) -> void {
    std::ranges::for_each(v, [](const T& x)->void{std::cout << x.first << "," << x.second << " ";});
    std::cout << "\n";
}

auto count_children(const Splitters& splitters, const std::pair<int64_t, int64_t>& cand) -> int64_t {
    const int64_t n = std::ranges::count_if(splitters.map, [&](const auto& pair) -> bool {
	    std::vector<std::pair<int64_t, int64_t>> parents = pair.second;
	    const bool found = std::ranges::find(parents, cand) != parents.end();
	    return found;
    });
    return n;
}

auto leaf_nodes(const Splitters& splitters) -> std::vector<std::pair<int64_t, int64_t>> {
    std::vector<std::pair<int64_t, int64_t>> leaves;
    std::ranges::for_each(splitters.map, [&] (const auto& pair) -> void {
	if (count_children(splitters, pair.first) == 0) {
	    leaves.push_back(pair.first);
	}
    });
    return leaves;
}



auto part_2(const std::string &input) -> Answer {
    Splitters splitters = parse_input(input);
    splitters.mark_activations();
    splitters.count_activations();
    // std::vector<std::pair<int64_t, int64_t>> leaves = leaf_nodes(splitters);
    return std::accumulate(splitters.map.begin(), splitters.map.end(), 0LL,
			   [&] (Answer acc, const auto& pair) -> Answer {
			       const int64_t f = 2 - count_children(splitters, pair.first);
			       return acc + (f * splitters.counts.at(pair.first));
			   });
}

void tests() // NOLINT(readability-function-cognitive-complexity)
{
    const std::string test_input = ".......S.......\n"
                                   "...............\n"
                                   ".......^.......\n"
                                   "...............\n"
                                   "......^.^......\n"
                                   "...............\n"
                                   ".....^.^.^.....\n"
                                   "...............\n"
                                   "....^.^...^....\n"
                                   "...............\n"
                                   "...^.^...^.^...\n"
                                   "...............\n"
                                   "..^...^.....^..\n"
                                   "...............\n"
                                   ".^.^.^.^.^...^.\n"
                                   "...............\n";

    Splitters splitters = parse_input(test_input);
    CHECK(splitters.source.first == 0);
    CHECK(splitters.source.second == 7);
    CHECK(splitters.map.size() == 22);

    splitters.mark_activations();
    CHECK(splitters.map.at({2, 7}).size() == 1);
    CHECK(splitters.map.at({2, 7}).size() == 1);
    CHECK(splitters.map.at({6, 7}).size() == 2);
    CHECK(splitters.map.at({14, 9}).empty());

    CHECK(part_1(test_input) == 21);

    splitters.count_activations();
    CHECK(splitters.counts.at({2, 7}) == 1);
    CHECK(splitters.counts.at({2, 7}) == 1);
    CHECK(splitters.counts.at({6, 7}) == 2);
    CHECK(splitters.counts.at({14, 9}) == 0);

    std::vector<std::pair<int64_t, int64_t>> leaves = leaf_nodes(splitters);
    UNUSED(leaves);
    CHECK(leaves.size() == 7);

    const std::string test_input_2 =
	".......S.......\n"
	"...............\n"
	".......^.......\n"
	"...............\n"
	"...............\n"
	"......^.^......\n"
	"...............\n";

    CHECK(part_1(test_input_2) == 3);

    splitters = parse_input(test_input_2);
    splitters.mark_activations();
    splitters.count_activations();

    CHECK(count_children(splitters, std::pair<int64_t, int64_t>({2, 7})) == 2);
    CHECK(count_children(splitters, std::pair<int64_t, int64_t>({5, 6})) == 0);
    CHECK(count_children(splitters, std::pair<int64_t, int64_t>({5, 8})) == 0);
    
    leaves = leaf_nodes(splitters);
    // printv(leaves);
    CHECK(std::ranges::find(leaves, std::pair<int64_t, int64_t>({2, 7})) == leaves.end());
    CHECK(std::ranges::find(leaves, std::pair<int64_t, int64_t>({5, 6})) != leaves.end());
    CHECK(std::ranges::find(leaves, std::pair<int64_t, int64_t>({5, 8})) != leaves.end());

    CHECK(part_2(test_input_2) == 4);


    const std::string test_input_3 =
	".......S.......\n"
	"...............\n"
	".......^.......\n"
	"...............\n"
	"......^.^......\n"
	"...............\n"
	".....^.^.^.....\n"
	"...............\n"
	"....^.^...^....\n"
	"...............\n";

    CHECK(part_2(test_input_3) == ((1*2)+(3*2)+(3*1+(1*2))));

    
    CHECK(part_2(test_input) == 40);
    
}

} // namespace day7
