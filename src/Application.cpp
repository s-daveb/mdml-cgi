/* Application.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Application.hpp"

#include <map>
#include <string>
#include <string_view>

using namespace mdml;

Application::Application(int argc, char* argv[], char* env[])
{
	this->create_env_dictionary(env);
}

/* For future use, in case the default constructor is not good enough
Application::~Application()
{

} */

const std::string&
Application::GetEnv(const std::string& key)
{
	return this->environment_variables[key];
}

void
Application::create_env_dictionary(char* env[])
{
	for (auto** current = env; (*current) != 0; ++current) {
		auto encoded_pair = std::string_view(*env);
		std::string key, value;
		size_t equal_character_pos;

		equal_character_pos = encoded_pair.find('=');
		if (equal_character_pos != encoded_pair.npos) {
			key = encoded_pair.substr(0, equal_character_pos);
			value = encoded_pair.substr(equal_character_pos + 1);
			this->environment_variables[key] = value;
		}
	}
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
