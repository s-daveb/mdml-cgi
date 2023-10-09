/* debuginfo.h
 * Copyright © 2020-2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "debuginfo.hpp"

#ifdef HAVE_EXECINFO_H
#include <cxxabi.h>
#include <execinfo.h>
#else
#include <boost/stacktrace.hpp>
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef __FreeBSD__
constexpr bool HAVE_FREEBSD = true;
#else
constexpr bool HAVE_FREEBSD = false;
#endif

namespace {
// Function to strip symbols enclosed within <>
std::string
stripTemplates(const std::string& input)
{
	std::string result;
	bool insideAngleBrackets = false;

	for (char c : input) {
		if (c == '<') {
			insideAngleBrackets = true;
			continue;
		}
		if (c == '>') {
			insideAngleBrackets = false;
			continue;
		}
		if (c == '+') {
			break;
		}

		if (insideAngleBrackets) {
			result += c;
		}
	}

	return result;
}
}
std::string
generate_stacktrace(unsigned short framesToRemove)
{
	std::stringstream buffer;
	char* demangled_name;

	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);

	size_t columns_to_print = 0;

	if (HAVE_FREEBSD) {
		columns_to_print = 2;
	}
	for (i = framesToRemove; i < frames; ++i) {
		std::string word;
		std::stringstream line_stream(strs[i]);
		std::vector<std::string> wordlist;

		// Create a list of words for this stack trace line
		while (line_stream >> word) {
			if (HAVE_FREEBSD && (word.find('<') != word.npos &&
			                     word.find('>') != word.npos)) {
				auto stripped_word = stripTemplates(word);
				word = stripped_word;
			}
			wordlist.push_back(word);
		}
		if (!columns_to_print) {
			columns_to_print = wordlist.size();
		}
		for (unsigned pos = 0; pos < columns_to_print; ++pos) {
			auto word = wordlist[pos];
			int status;

			char* demangled_word = abi::__cxa_demangle(
			    word.c_str(), nullptr, nullptr, &status
			);

			if (status == 0) {
				buffer << demangled_word << '\t';
				std::free(demangled_word);
			} else {
				buffer << word << '\t';
			}
		}

		buffer << std::endl;
	}

	free(strs);

	return buffer.str();
}

void
print_cmdline(int argc, const char* argv[])
{
	int i;

	std::cout << "Command-line received" << std::endl;
	for (i = 0; i < argc; ++i)
		std::cout << argv[i] << " ";
	std::cout << std::endl;
}

// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :

