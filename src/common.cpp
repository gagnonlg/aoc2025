#include <algorithm>
#include <istream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "common.hpp"

auto split_at(std::istream& stream, char delim, bool allow_empty) -> std::vector<std::string>
{
    (void)allow_empty;
    std::vector<std::string> lines;
    std::string buf;
    while(std::getline(stream, buf, delim)) {
	if (allow_empty ||
	    std::ranges::any_of(buf, [&](char this_char) -> bool {return this_char != delim;} )) {
	    lines.push_back(std::move(buf));
	}
	buf.clear();
    }
    return lines;
}


auto split_at(const std::string& str, char delim, bool allow_empty) -> std::vector<std::string> 
{
    std::stringstream stream(str);
    return split_at(stream, delim, allow_empty);
}


auto split_lines(const std::string& str) -> std::vector<std::string> 
{
    return split_at(str, '\n');
}

