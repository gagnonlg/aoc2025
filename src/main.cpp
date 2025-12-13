#include <cstdlib>
#include <exception>
#include <iostream>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "common.hpp"
#include "days.hpp" // NOLINT(misc-include-cleaner)

constexpr int MAX_DAY = 12;

template <int N,
	  typename T = std::conditional_t<Day<N>::value, Day<N>, void>,
	  typename U = std::conditional_t<Day<N + 1>::value, Day<N + 1>, void>>
struct RunnerImpl;

template <int N>
struct RunnerImpl<N, Day<N>, void> {
    // Base case
    auto call(std::optional<int> day, std::optional<int> part) -> int { // NOLINT
	if (!day || *day == N) {
	    return Day<N>().run(part);
	}
	return EXIT_SUCCESS;
    }
    auto match(int n) -> bool {
	return n == N;
    }
};

template <int N>
struct RunnerImpl<N, Day<N>, Day<N+1>> {
    using Next = RunnerImpl<N + 1>;
    auto call(std::optional<int> day, std::optional<int> part) -> int {
	if (!day || *day == N) {
	    Day<N>().run(part);
	}
	Next().call(day, part);
	return EXIT_SUCCESS;
    }
    auto match(int n) -> bool {
	return (n == N) || Next().match(n);
    }
};

template <int N>
struct RunnerImpl<N, void, void> {
    static_assert(false, "This day is not implemented yet");
};

template <int N>
struct RunnerImpl<N, void, Day<N+1>> {
    static_assert(false, "Impossible: Day<N+1> exists but not Day<N>!");

};

using Runner = RunnerImpl<1>;

auto get_args(std::span<char *> argv) -> std::pair<std::optional<int>, std::optional<int>>
{
    const std::optional<int> null = {};

    if (argv.size() == 1) {
	return std::make_pair(null, null);
    }

    if (argv.size() == 2) {
	const int day = std::stoi(argv[1]);
	return std::make_pair(day, null);
    }

    if (argv.size() == 3) {
	const int day = std::stoi(argv[1]);
	const int part = std::stoi(argv[2]);
	return std::make_pair(day, part);
    }

    throw std::runtime_error("Usage: " + std::string(argv[0]) + " [DAY] [PART]");
}

int main(int argc, char *argv[]) // NOLINT
{
    try {
	const std::pair<std::optional<int>,
			std::optional<int>> args
	    = get_args(std::span<char *>(argv, argc));

	std::optional<int> day = args.first;
	std::optional<int> part = args.second;

	Runner runner;

	if (day) {
	    if (*day < 1 || *day > MAX_DAY) {
		throw std::runtime_error(
		    "Invalid day: " + std::to_string(*day));
	    }
	    if (!runner.match(*day)) {
		throw std::runtime_error(
		    "Day " + std::to_string(*day) + " not implemented");
	    }
	}

	if (part) {
	    if (*part != 1 && *part != 2) {
		throw std::runtime_error(
		    "Invalid part: " + std::to_string(*part));
	    }
	}
	return runner.call(day, part);
    } catch (const std::exception& e) {
	std::cerr << "FATAL: Uncaught exception: " << e.what() << '\n';
	return EXIT_FAILURE;
    }
}
