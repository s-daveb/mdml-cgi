/* exceptions.hpp
 * Copyright © 2020 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "debuginfo.hpp"
#include "exception.hpp"

#include <algorithm>
#include <exception>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

#define NOT_IMPLEMENTED std::runtime_error("Unimplemented Method");

using mdml::exception;

exception::exception(
    const std::optional<std::exception> innerException,
    const std::optional<std::string> backtrace
)
    : std::exception(), stacktrace(std::nullopt), innerException(std::nullopt)
{
	if (!backtrace.has_value()) {
		this->stacktrace = generate_stacktrace(2);
	} else {
		this->stacktrace = backtrace;
	}

	if (innerException.has_value()) {
		this->innerException = *(innerException);
	}
}

const char*
exception::what() const noexcept
{
	return innerException->what();
}

const char*
exception::getStacktrace() const noexcept
{
	return stacktrace->c_str();
}

// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
