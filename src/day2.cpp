#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "common.hpp"

namespace day2 {

auto generate_invalid(const std::string& id_str, size_t n = 2) -> std::string {
    CHECK(n >= 2);
    std::string inv = id_str;
    for (size_t i = 1; i < n; i++) {
	inv += id_str;
    }
    return inv;
}

auto generate_all_invalids(const std::string& start, const std::string& end, bool single = true) -> std::vector<Answer> {
    const Answer istart = std::stoll(start);
    const Answer iend = std::stoll(end);
    std::vector<Answer> invalids;

    for (size_t i = 2; i <= (single ? 2 : end.size()); i++) {
	for (Answer j = 1; j <= iend; j++) {
	    const Answer cand = std::stoll(generate_invalid(std::to_string(j), i));
	    if (cand < istart) {
		continue;
	    }
	    if (cand > iend) {
		break;
	    }
	    if (std::ranges::find(invalids, cand) == invalids.end()) {
		invalids.push_back(cand);
	    }
	}
    }
    return invalids;
}

auto sum_all_invalids(const std::pair<std::string, std::string>& range, bool single = true) -> Answer {
    const std::vector<Answer> invalids = generate_all_invalids(range.first, range.second, single);
    return std::accumulate(invalids.begin(), invalids.end(), 0LL);
}

auto to_range(const std::string& entry) -> std::pair<std::string, std::string> {
    std::vector<std::string> vecpair = split_at(entry, '-');
    CHECK(vecpair.size() == 2);
    return std::make_pair(vecpair[0], vecpair[1]);
}

auto part_1(const std::string& input) -> Answer
{
    const std::vector<std::string> str_entries = split_at(input, ',');

    std::vector<std::pair<std::string, std::string>> entries;
    std::ranges::transform(str_entries, std::back_inserter(entries), to_range);
    
    std::vector<Answer> partialsums;
    std::ranges::transform(entries, std::back_inserter(partialsums), std::bind_back(sum_all_invalids, true));

    return std::accumulate(partialsums.begin(), partialsums.end(), 0LL);
}

auto part_2(const std::string& input) -> Answer
{
    const std::vector<std::string> str_entries = split_at(input, ',');

    std::vector<std::pair<std::string, std::string>> entries;
    std::ranges::transform(str_entries, std::back_inserter(entries), to_range);
    
    std::vector<Answer> partialsums;
    std::ranges::transform(entries, std::back_inserter(partialsums), std::bind_back(sum_all_invalids, false));

    return std::accumulate(partialsums.begin(), partialsums.end(), 0LL);
}

void tests() // NOLINT(readability-function-cognitive-complexity)
{
    CHECK(generate_invalid("1") == "11");

    CHECK(generate_all_invalids("11", "22") == std::vector<Answer>({11, 22}));
    CHECK(generate_all_invalids("95", "115") == std::vector<Answer>({99}));
    CHECK(generate_all_invalids("998", "1012") == std::vector<Answer>({1010}));
    CHECK(generate_all_invalids("1188511880", "1188511890") == std::vector<Answer>({1188511885}));
    CHECK(generate_all_invalids("222220", "222224") == std::vector<Answer>({222222}));
    CHECK(generate_all_invalids("1698522", "1698528").empty());
    CHECK(generate_all_invalids("446443", "446449") == std::vector<Answer>({446446}));
    CHECK(generate_all_invalids("38593856", "38593862") == std::vector<Answer>({38593859}));
    CHECK(generate_all_invalids("565653", "565659").empty());
    CHECK(generate_all_invalids("824824821", "824824827").empty());
    CHECK(generate_all_invalids("2121212118", "2121212124").empty());

    CHECK(sum_all_invalids(std::make_pair("11", "22")) == (11 + 22));

    CHECK(to_range("11-22") == std::make_pair("11", "22"));

    const std::string test_input_1 = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124";

    CHECK(part_1(test_input_1) == 1227775554);

    CHECK(generate_invalid("9", 3) == "999");
    CHECK(generate_invalid("2", 6) == "222222");

    CHECK(generate_all_invalids("11", "22", false) == std::vector<Answer>({11, 22}));
    CHECK(generate_all_invalids("95", "115", false) == std::vector<Answer>({99, 111}));
    CHECK(generate_all_invalids("998", "1012", false) == std::vector<Answer>({1010, 999}));
    CHECK(generate_all_invalids("1188511880", "1188511890", false) == std::vector<Answer>({1188511885}));
    CHECK(generate_all_invalids("222220", "222224", false) == std::vector<Answer>({222222}));
    CHECK(generate_all_invalids("1698522", "1698528", false).empty());
    CHECK(generate_all_invalids("446443", "446449", false) == std::vector<Answer>({446446}));
    CHECK(generate_all_invalids("38593856", "38593862", false) == std::vector<Answer>({38593859}));
    CHECK(generate_all_invalids("565653", "565659", false) == std::vector<Answer>({565656}));
    CHECK(generate_all_invalids("824824821", "824824827", false) == std::vector<Answer>({824824824}));
    CHECK(generate_all_invalids("2121212118", "2121212124", false) == std::vector<Answer>({2121212121}));

    CHECK(part_2(test_input_1) == 4174379265);

    
}

     
} // namespace day2

