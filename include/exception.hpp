/* exceptions.hpp
 * Copyright © 2020 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "debuginfo.hpp"
#include "types.hpp"

#include <algorithm>
#include <exception>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

#define NOT_IMPLEMENTED std::runtime_error("Unimplemented Method");

#define THROW_EXCEPTION(message)                                                \
	throw mdml::exception(message, generate_stacktrace(2))

namespace mdml {

class exception : public std::exception {
    public:
	explicit exception(
	    optional_reference<std::exception> innerException = std::nullopt,
	    const std::optional<std::string> backtrace = std::nullopt
	);
	exception(const std::runtime_error& e);

	virtual const char* what() const noexcept override;

	const char* getStacktrace() const noexcept;

    private:
	void build_what_message();

	std::string what_message;
	std::optional<std::string> stacktrace;
	std::exception_ptr inner_exception_ptr;
};
}
// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
