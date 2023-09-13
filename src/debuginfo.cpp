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
#ifdef HAVE_EXECINFO_H
	void* callstack[128];

	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	for (i = framesToRemove; i < frames; ++i) {
		buffer << strs[i] << std::endl;
	}
	free(strs);
#else
	buffer << boost::stacktrace::stacktrace() << std::flush;
#endif
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
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :

