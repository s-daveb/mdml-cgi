/* CgiRequestHandler.cpp
 * Copyright © 2023 Saul D. Beniuqez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "CgiRequestHandler.hpp"
#include "ICgiRequestHandler.hpp"
#include "IRouteHandler.hpp"
#include "exception.hpp"
#include "types.hpp"

#include <iostream>
#include <sstream>
#include <string>

using namespace mdml;

void CgiRequestHandler::ImportRoutes(
    Dictionary<IRouteHandler_ptr> &new_routes) {
    this->routes = std::move(new_routes);
}

Result<std::string>
CgiRequestHandler::ProcessRequest(RequestInfo &request_info) {
    try {
	/* Parse REQUEST_URI to get the route's basename */
	auto request_URI = request_info.uri;
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
	    auto result = route_handler->Process(page_name, request_URI);

	    if (result.IsError) {
		auto except = mdml::exception(result.ErrorData.c_str());
		throw except;
	    }

	    return result;
	} else {
	    std::stringstream buffer;
	    buffer << "Unknown route: " << page_name << std::endl;
	    return {ERROR, buffer.str()};
	    // throw mdml::exception(buffer.str().c_str());
	}
    } catch (const mdml::exception &except) {
	throw except;
    } catch (const std::exception &except) {
	throw mdml::exception(except);
    }
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
