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
	exception(
	    const std::optional<std::exception> innerException = std::nullopt,
	    const std::optional<std::string> backtrace = std::nullopt
	);
	const char* what() const noexcept override;

	const char* getStacktrace() const noexcept;

    private:
	std::optional<std::string> stacktrace;
	std::optional<std::exception> innerException;
};
}
// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
