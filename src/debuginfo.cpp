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

std::string
generate_stacktrace(unsigned short framesToRemove)
{
	std::stringstream buffer;
	char* demangled_name;

	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);

	for (i = framesToRemove; i < frames; ++i) {
		std::stringstream line_stream(strs[i]);
		std::string word;

		// Split the demangled name by spaces and demangle each word
		while (line_stream >> word) {
			if (word.find("0x") == std::string::npos) {
				int status;
				char* demangled_word = abi::__cxa_demangle(
				    word.c_str(), nullptr, nullptr, &status
				);

				if (status == 0) {
					buffer << '\t' << demangled_word << ' ';
					std::free(demangled_word);
				} else {
					buffer << word << ' ';
				}
			} else {
				buffer << word << ' ';
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

