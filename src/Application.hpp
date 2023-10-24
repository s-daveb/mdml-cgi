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
#include <vector>

namespace mdml {

class Application {
    public:
	Application(int argc, c::const_string argv[], c::const_string env[]);

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	virtual ~Application();

	inline const std::vector<const std::string>& ArgumentsVector()
	{
		return this->arguments;
	}
	inline const Dictionary<const std::string>& EnvironDictionary()
	{
		return this->environment_variables;
	}

	const std::string& GetEnv(const std::string& key);

    private:
	static count_t instance_count;

	void parse_arguments(int argc, c::const_string argv[]);
	void create_env_dictionary(c::const_string envp[]);

	std::vector<const std::string> arguments;
	Dictionary<const std::string> environment_variables;
};
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=81 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
