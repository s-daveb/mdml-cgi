/* exceptions.hpp
 * Copyright © 2020 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "exception.hpp"

#include "debuginfo.hpp"
#include "types.hpp"

#include <algorithm>
#include <exception>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

#define NOT_IMPLEMENTED std::runtime_error("Unimplemented Method");

using mdml::exception;

exception::exception(
    optional_reference<std::exception> nestedException,
    const std::optional<std::string> backtrace
)
    : std::exception(), stacktrace(std::nullopt), inner_exception_ptr(nullptr)
{
	if (nestedException.has_value()) {
		this->inner_exception_ptr =
		    std::make_exception_ptr(nestedException.value().get());
	}

	if (!backtrace.has_value()) {
		this->stacktrace = generate_stacktrace(2);
	} else {
		this->stacktrace = backtrace;
	}

	build_what_message();
}

exception::exception(const std::runtime_error& e)
    : std::exception(), inner_exception_ptr(), stacktrace(std::nullopt)
{
	inner_exception_ptr = std::make_exception_ptr(e);
	this->stacktrace = generate_stacktrace(2);

	build_what_message();
}

const char*
exception::what() const noexcept
{
	return this->what_message.c_str();
}

const char*
exception::getStacktrace() const noexcept
{
	return this->stacktrace->c_str();
}

void
exception::build_what_message()
{
	std::stringstream buffer;

	if (inner_exception_ptr) {
		try {
			std::rethrow_exception(inner_exception_ptr);
		} catch (const std::exception& rethrown) {
			buffer << rethrown.what();
		}
	} else {
		buffer << "An error has ocurred. (No Inner exception)";
	}
	buffer << std::endl;

	if (this->stacktrace.has_value()) {
		buffer << std::endl << "Backtrace: " << std::endl;
		buffer << this->stacktrace.value() << std::endl;
	}

	what_message = buffer.str();
}

// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
