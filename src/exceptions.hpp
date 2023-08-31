/* exceptions.hpp
 * Copyright © 2020 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <exception>
#include <stdexcept>

#define NOT_IMPLEMENTED std::runtime_error("Unimplemented Method");

namespace mdml {
class exception : public std::exception {
    public:
	exception(
	    const std::exception& innerException, const std::string& backtrace
	)
	    : stacktrace(backtrace), innerException(innerException)
	{
	}

	const char* what() const noexcept override
	{
		return innerException.what();
	}

	const char* getStacktrace() const noexcept { return stacktrace.c_str(); }

    private:
	inline std::string
	generate_stacktrace()
	{
		std::stringstream buffer;
	#ifdef HAVE_EXECINFO_H
		void* callstack[128];

		int i, frames = backtrace(callstack, 128);
		char** strs = backtrace_symbols(callstack, frames);
		for (i = 0; i < frames; ++i) {
			buffer << strs[i] << std::endl;
		}
		free(strs);
	#else
		buffer << boost::stacktrace::stacktrace() << std::flush;
	#endif
		return buffer.str();
	}

	std::string stacktrace;
	std::exception innerException;
};
}
// Macro to throw an exception with a generated stack trace
#define THROW_EXCEPTION(message)                                                \
	throw QWException(message, generate_stacktrace())

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
