/* Application.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Application.hpp"
#include "exception.hpp"
#include "types.hpp"

#include <string>
#include <string_view>
#include <vector>

using namespace mdml;

count_t Application::instance_count = 0;
Application* Application::instance_ptr = nullptr;

Application::Application(int argc, c::const_string argv[], c::const_string env[])
    : arguments(), environment_variables()
{
	if (Application::instance_count != 0) {
		std::stringstream buffer;
		buffer << this->instance_count;

		auto fatal_exception = std::logic_error(
		    "Cannot instantiate more than one "
		    "mdml::Application class at a time.\n"
		    "	instance_count = " +
		    buffer.str()
		);
		throw mdml::exception(fatal_exception);
	}

	if (argv == nullptr || env == nullptr) {
		std::string problem = (argv == nullptr) ? "argv " : "envp ";
		auto message = "Cannot instantiate the application class "
		               "with null " +
		               problem + "parameter";

		auto fatal_exception = std::logic_error(message);
		throw mdml::exception(fatal_exception);
	}

	++(Application::instance_count);
	this->parse_arguments(argc, argv);
	this->create_env_dictionary(env);

	Application::instance_ptr = this;
}

Application::~Application()
{
	--(Application::instance_count);
	Application::instance_ptr = nullptr;
}

void
Application::parse_arguments(int argc, c::const_string argv[])
{
	for (count_t index = 0; index < argc; ++index) {
		this->arguments.push_back(argv[index]);
	}
}

void
Application::create_env_dictionary(c::const_string envp[])
{
	for (count_t num = 0; envp[num] != nullptr; ++num) {
		auto encoded_pair = std::string_view(envp[num]);

		size_t equal_character_pos;
		auto not_found = encoded_pair.npos;

		equal_character_pos = encoded_pair.find('=');
		if (equal_character_pos != not_found) {
			auto key = std::string(
			    encoded_pair.substr(0, equal_character_pos)
			);
			auto val = std::string(
			    encoded_pair.substr(equal_character_pos + 1)
			);

			this->environment_variables.insert(
			    std::make_pair(key, val)
			);
		}
	}
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
