/* CgiApplication-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "CgiApplication.hpp"
#include "IRouteHandler.hpp"
#include "exception.hpp"

#include "tests-common.hpp"
#include <catch2/matchers/catch_matchers_string.hpp>

namespace {
struct simulated_launch {
	static const char* argv[];
	static const char* env[];
};
inline const char* simulated_launch::argv[] = { "param1", "param2", "param3" };
inline const char* simulated_launch::env[] = {
	"PATH=/usr/bin",
	"VAR2=TWO",
	"REQUEST_URI=markdown?msg=hello-world",
	nullptr
};
}

BEGIN_TEST_SUITE("CgiApplication")
{
	using namespace mdml;
	struct TestFixture {
		const char* envp[4] = { "PATH=/usr/bin",
			                "VAR2=TWO",
			                "REQUEST_URI=markdown?msg=hello-world",
			                nullptr };
		TestFixture() : cgi_app(1, simulated_launch::argv, envp) {}

		CgiApplication cgi_app;
	};

	TEST("CgiApplication Constructor Test")
	{

		CgiApplication test(
		    1, simulated_launch::argv, simulated_launch::env
		);
	};

	TEST("CgiApplication::ProcessRequest no REQUEST_URI variable")
	{
		const char* no_request_env[] = { "PATH=/usr/bin",
			                         "VAR2=TWO",
			                         nullptr };
		CgiApplication test(1, simulated_launch::argv, no_request_env);
		REQUIRE_THROWS_AS(
		    [&]() { test.ProcessRequest(); }(), mdml::exception
		);
	}

	FIXTURE_TEST("CgiApplication ProcessRequest with valid route")
	{
		// Define a test oute handler for the "/markdown" route
		class ExampleRouteHandler : public IRouteHandler {
		    public:
			virtual Result<std::string> Process(
			    const std::string& name,
			    const std::string& request_uri
			) override
			{
				// Implement test logic for the route
				// handler
				return { mdml::NO_ERROR, "Processed" };
			}
			virtual ~ExampleRouteHandler() = default;
		};

		// Add the test route handler to the Routes dictionary
		auto handler = CgiApplication::make_ptr<ExampleRouteHandler>();
		cgi_app.Routes["markdown"] = handler;

		auto result = cgi_app.ProcessRequest();

		// Add assertions to check the result
		CHECK(result.ErrorData == "Processed");
		REQUIRE(result.IsError == false);
	}

	FIXTURE_TEST("CgiApplication ProcessRequest with unknown route")
	{

		// Remove the "markdown" route from the Routes
		// dictionary
		for (auto& [key, value] : cgi_app.Routes) {
			if (value.get() != nullptr) {
				value.reset();
			}
			cgi_app.Routes.erase(key);
		}

		auto result = cgi_app.ProcessRequest();

		// Add assertions to check the result
		CHECK(result.ErrorData != "Success");
		REQUIRE(result.IsError == true);
	}

	FIXTURE_TEST("CgiApplication ProcessRequest Route throws exception")
	{
		// Define a test oute handler for the "/markdown" route
		class BadRouteHandler : public IRouteHandler {
		    public:
			virtual Result<std::string> Process(
			    const std::string& name,
			    const std::string& request_uri
			) override
			{
				throw std::runtime_error("ERROR");
				return { mdml::ERROR,
					 "This line never executes" };
			}
			virtual ~BadRouteHandler() = default;
		};
		auto handler = CgiApplication::make_ptr<BadRouteHandler>();
		cgi_app.Routes["markdown"] = handler;

		REQUIRE_THROWS_AS(
		    [&]() { cgi_app.ProcessRequest(); }(), mdml::exception
		);
	}
}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
