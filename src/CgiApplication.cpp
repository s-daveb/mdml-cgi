/* CgiApplication.cpp
 * Copyright © 2023 Saul D. Beniuqez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Application.hpp"
#include "CgiApplication.hpp"
#include "IRouteHandler.hpp"
#include "exception.hpp"
#include "types.hpp"

#include <iostream>
#include <sstream>
#include <string>

using namespace mdml;

struct MarkdownTemplateRequestHandler : public IRouteHandler {};

CgiApplication::CgiApplication(
    int argc, c::const_string argv[], c::const_string env[]
)
    : Application(argc, argv, env)
    , routes()
#ifdef TESTING
    , Routes(this->routes)
#endif
{
}
CgiApplication::~CgiApplication()
{
	for (auto& [key, val_ptr] : this->routes) {
		if (val_ptr.get() != nullptr) {
			val_ptr.reset();
		}
	}
}

Result<std::string>
CgiApplication::ProcessRequest()
{
	try {
		auto& environment = Application::environment_variables;

		// Check if REQUEST_URI is in the environment map
		if (environment.find("REQUEST_URI") == environment.end()) {
			throw mdml::exception("No REQUEST_URI in Environment!");
		}
		/* Parse REQUEST_URI to get the route's basename */
		auto request_URI = environment["REQUEST_URI"];
		auto question_pos = request_URI.find('?');
		auto ampersand_pos = request_URI.find('&');
		auto page_name = request_URI;
		auto not_found = std::string::npos;

		if (question_pos == not_found) {
			if (ampersand_pos != not_found) {
				question_pos = ampersand_pos;
			}
		}
		if (question_pos != not_found) {
			page_name = request_URI.substr(0, question_pos);
		}
		if (routes.find(page_name) != routes.end()) {
			auto route_handler = routes[page_name];
			auto result =
			    route_handler->Process(page_name, request_URI);

			if (result.IsError) {
				auto except =
				    mdml::exception(result.ErrorData.c_str());
				throw except;
			}

			return result;
		} else {
			std::stringstream buffer;
			buffer << "Unknown route: " << page_name << std::endl;
			return { ERROR, buffer.str() };
			// throw mdml::exception(buffer.str().c_str());
		}
	} catch (const std::exception& except) {
		throw mdml::exception(except);
	}
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
