#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>


#include "common.hpp"

namespace day6 {

struct Problem {
    using Operation = std::variant<std::plus<Answer>, std::minus<Answer>, std::multiplies<Answer>>;
    std::vector<Answer> numbers; // NOLINT(misc-non-private-member-variables-in-classes)
    Operation operation; // NOLINT(misc-non-private-member-variables-in-classes)
    explicit Problem(Operation opr) : operation{opr} {}

    [[nodiscard]]
    auto answer() const -> Answer {
	return std::visit([&](const auto& opr) -> Answer {
	    return std::accumulate(std::next(numbers.begin()), numbers.end(), numbers.front(), opr);
	}, operation);
    }
    
};

auto parse_input(const std::string& input) -> std::vector<Problem> {
    std::vector<Problem> problems;

    std::vector<std::string> lines = split_lines(input);
    CHECK(input.size() >= 2);

    // Do operations firsts
    std::ranges::transform(split_at(lines.back(), ' '), std::back_inserter(problems),
			   [] (const std::string& opstr) -> Problem {
			       if (opstr.size() != 1) {
				   throw std::invalid_argument("Invalid opstr: " + opstr);
			       }
			       switch (opstr[0]) {
			       case '+': return Problem(std::plus<Answer>{});
			       case '-': return Problem(std::minus<Answer>{});
			       case '*': return Problem(std::multiplies<Answer>{});
			       default:
				   throw std::invalid_argument("Invalid opstr: " + opstr);
			       }
			   });

    std::ranges::for_each(lines.begin(), std::prev(lines.end()),
			  [&] (const std::string& line) -> void {
			      std::vector<std::string> elems = split_at(line, ' ');
			      for (size_t idx = 0; idx < elems.size(); idx++) {
				  problems.at(idx).numbers.push_back(std::stoll(elems.at(idx)));
			      }
			  });

    return problems;

}

auto part_1(const std::string& input) -> Answer
{
    const std::vector<Problem> problems = parse_input(input);
    return std::accumulate(problems.begin(), problems.end(), 0LL,
			   [] (Answer acc, const Problem& prob) -> Answer {
			       return acc + prob.answer();
			   });
}

auto parse_input_2(const std::string& input) -> std::vector<Problem> {
    std::vector<Problem> problems;

    std::vector<std::string> lines = split_lines(input);
    CHECK(input.size() >= 2);

    // Do operations firsts
    std::ranges::transform(split_at(lines.back(), ' '), std::back_inserter(problems),
			   [] (const std::string& opstr) -> Problem {
			       if (opstr.size() != 1) {
				   throw std::invalid_argument("Invalid opstr: " + opstr);
			       }
			       switch (opstr[0]) {
			       case '+': return Problem(std::plus<Answer>{});
			       case '-': return Problem(std::minus<Answer>{});
			       case '*': return Problem(std::multiplies<Answer>{});
			       default:
				   throw std::invalid_argument("Invalid opstr: " + opstr);
			       }
			   });


    const size_t width = lines.front().size();
    CHECK(std::ranges::all_of(lines.begin(), std::prev(lines.end()),
			     [=](const std::string& str)->bool{return str.size() == width;}));
    
    
    std::vector<std::string> atoms;
    for (size_t iwidth = 0; iwidth < width; iwidth++) {
	if (atoms.size() <= iwidth) {
	    atoms.emplace_back();
	}
	for (size_t iline = 0; iline < lines.size() - 1; iline++) {
	    atoms.back().push_back(lines.at(iline).at(iwidth));
	}
    }

    size_t iprob = 0;
    size_t iatom = 0;
    while (iprob < problems.size() && iatom < atoms.size()) {
	if (std::ranges::all_of(atoms.at(iatom), [](char thec) -> bool {return thec == ' ';})) {
	    iprob++;
	} else {
	    problems.at(iprob).numbers.push_back(std::stoll(atoms.at(iatom)));
	}
	iatom++;
    }

    return problems;

}


auto part_2(const std::string& input) -> Answer
{
    const std::vector<Problem> problems = parse_input_2(input);
    return std::accumulate(problems.begin(), problems.end(), 0LL,
			   [] (Answer acc, const Problem& prob) -> Answer {
			       return acc + prob.answer();
			   });
}

void tests() // NOLINT(readability-function-cognitive-complexity)
{
    const std::string test_input =
	"123 328  51 64 \n"
	" 45 64  387 23 \n"
	"  6 98  215 314\n"
	"*   +   *   +\n";

    const std::vector<Problem> test_problems = parse_input(test_input);
    CHECK(test_problems.size() == 4);
    CHECK(std::ranges::all_of(test_problems, [](const auto& prob)->bool{return prob.numbers.size() == 3;}));

    CHECK(std::holds_alternative<std::multiplies<Answer>>(test_problems[0].operation));
    CHECK(test_problems[0].numbers == std::vector<Answer>({123, 45, 6}));
    CHECK(test_problems[0].answer() == 33210);
    CHECK(test_problems[1].answer() == 490);
    CHECK(test_problems[2].answer() == 4243455);
    CHECK(test_problems[3].answer() == 401);

    CHECK(part_1(test_input) == 4277556);

    const std::vector<Problem> test_problems_2 = parse_input_2(test_input);
    CHECK(test_problems_2.size() == 4);
    CHECK(test_problems_2.back().numbers.size() == 3);
    CHECK(test_problems_2.back().numbers[0] == 623);
    CHECK(test_problems_2.back().numbers[1] == 431);
    CHECK(test_problems_2.back().numbers[2] == 4);
    CHECK(test_problems_2[0].answer() == 8544);
    CHECK(test_problems_2[1].answer() == 625);
    CHECK(test_problems_2[2].answer() == 3253600);
    CHECK(test_problems_2[3].answer() == 1058);

    CHECK(part_2(test_input) == 3263827);
}

     
} // namespace day6

