/* Application.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "types.hpp"

#include <string>

namespace mdml {

class Application {
    public:
	explicit Application(int argc, char* argv[], char* env[]);
	virtual ~Application() = default;

	const std::string& GetEnv(const std::string& key);

    private:
	void create_env_dictionary(char* env[]);
	Dictionary<std::string> environment_variables;
};
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=81 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
