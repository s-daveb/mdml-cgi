/* CGIhandler.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Application.hpp"
#include "types.hpp"

#include <memory>
#include <string>
#include <vector>

namespace mdml {

class IRouteHandler;

class CgiApplication : public Application {

    public:
	using RouteDictionary = Dictionary<managed::ptr<IRouteHandler>>;

	CgiApplication(int argc, c::const_string argv[], c::const_string env[]);
	virtual ~CgiApplication();

	Result<std::string> ProcessRequest();

	inline void ImportRoutes(RouteDictionary& route_collection)
	{
		this->routes = std::move(route_collection);
	}

#ifdef TESTING
	/* Expose a public reference to the Routes map, but only for unit-testing
	 * builds */
	RouteDictionary& Routes;
#endif
    protected:
	RouteDictionary routes;
};

}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=81 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
