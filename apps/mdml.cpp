/* ml.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "CgiApplication.hpp"
#include "MarkdownRouteHandler.hpp"
#include "libmdml.hpp"
#include "types.hpp"

#include <functional>
#include <iostream>
#include <optional>
#include <string>

namespace {
using string_result = mdml::Result<std::string>;
}

int
handle_error(
    const string_result* error_data = nullptr,
    const std::exception* except = nullptr
)
{
	auto result = 0;
	std::cerr << "An error occurred" << std::endl;
	if (error_data && error_data->IsError) {
		std::cerr << error_data->ErrorData << std::endl;
		result |= 0x001;
	}

	if (except) {
		std::cerr << except->what() << std::endl;
		result |= 0x010;
	}

	return result;
}
int
main(int argc, const char* argv[], const char* envp[])
{

	try {
		auto app = mdml::CgiApplication(argc, argv, envp);
		auto markdown_routes =
		    mdml::MarkdownRouteHandler::GenerateRoutes(
			"/usr/local/www/content",
			"/usr/local/www/templates/main.thmtl"
		    );

		app.ImportRoutes(markdown_routes);

		auto result = app.ProcessRequest();

		if (result.IsError) {
			return handle_error(&result);
		}
	} catch (std::exception& except) {
		return handle_error(nullptr, &except);
	}
	return 0;
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
