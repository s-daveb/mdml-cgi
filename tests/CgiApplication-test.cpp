/* CgiApplication-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "tests-common.hpp"

#include "CgiApplication.hpp"
#include "IRouteHandler.hpp"

struct simulated_launch {
	static const char* argv[];
	static const char* env[];
};
inline const char* simulated_launch::argv[] = { "param1", "param2", "param3" };
inline const char* simulated_launch::env[] = {
	"PATH=/usr/bin",
	"VAR2=TWO",
	"REQUEST_URI=markdown?msg=hello-world"
};

using namespace mdml;

TEST_CASE("CgiApplication Unit Tests")
{
	CgiApplication cgiHandler(
	    1, simulated_launch::argv, simulated_launch::env
	);

	SECTION("ProcessRequest with valid route")
	{

		// Define a test route handler for the "/markdown" route
		class ExampleRouteHandler : public IRouteHandler {
		    public:
			virtual Result<std::string> Process(
			    const std::string& name,
			    const std::string& request_uri
			) override
			{
				// Implement test logic for the route handler
				return { mdml::NO_ERROR, "Success" };
			}
			virtual ~ExampleRouteHandler() = default;
		};

		// Add the test route handler to the Routes dictionary
		auto handler = CgiApplication::make_ptr<ExampleRouteHandler>();
		cgiHandler.Routes["/markdown"] = handler;

		auto result = cgiHandler.ProcessRequest();

		// Add assertions to check the result
		CHECK(result.IsError == false);
		CHECK(result.ErrorData == "Success");
	}

	SECTION("ProcessRequest with unknown route")
	{
		// Remove the "/markdown" route from the Routes dictionary
		cgiHandler.Routes.erase("/markdown");

		cgiHandler.ProcessRequest();

		// Add assertions to check the result for the unknown route
		/*REQUIRE( Add assertions for the test case &/);*/
	}
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
