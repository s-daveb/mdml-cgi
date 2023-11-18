/* CgiApplication-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "tests-common.hpp"

#include "IRouteHandler.hpp"
#include "MarkdownRouteHandler.hpp"

#include <catch2/matchers/catch_matchers_string.hpp>
#include <iostream>
#include <sstream>

namespace {
struct simulated_launch {
	static const char* argv[];
	static const char* env[];
};
inline const char* simulated_launch::argv[] = { "param1", "param2", "param3" };
inline const char* simulated_launch::env[] = {
	"PATH=/usr/bin",
	"VAR2=TWO",
	"REQUEST_URI=markdown?src=index.md"
};
}

using namespace mdml;

BEGIN_TEST_SUITE("MarkdownRouteHandler Unit Tests")
{

	TEST("MarkdownRouteHandler Construction and allocation")
	{

		REQUIRE_NOTHROW([]() { MarkdownRouteHandler test; }());
	}

	struct TestFixture {
		MarkdownRouteHandler test_obj;
	};

	FIXTURE_TEST("MarkdownRouteHandler LoadHtml")
	{
		MarkdownRouteHandler test_obj;

		SECTION("File exists")
		{
			REQUIRE_NOTHROW([&]() {
				test_obj.LoadTemplate("data/test.thtml");
			}());

			REQUIRE(false == test_obj.GetHtmlData().empty());
		}
		SECTION("File does not exists")
		{
			REQUIRE_THROWS([&]() {
				test_obj.LoadTemplate("not-found.txt");
			}());
		}
	}
	FIXTURE_TEST("MarkdownRouteHandler LoadMarkdown")
	{
		MarkdownRouteHandler test_obj;

		SECTION("File exists")
		{
			REQUIRE_NOTHROW([&]() {
				test_obj.LoadMarkdown("data/test.md");
			}());

			REQUIRE(false == test_obj.GetMarkdownData().empty());
		}
		SECTION("File does not exists")
		{
			REQUIRE_THROWS([&]() {
				test_obj.LoadMarkdown("not-found.txt");
			}());
		}
	}
	FIXTURE_TEST("MarkdownRouteHandler Process")
	{
		std::stringstream buffer;
		test_obj.OutputStream = buffer;

		REQUIRE_NOTHROW([&]() {
			test_obj.LoadTemplate("data/test.thtml");
			test_obj.LoadMarkdown("data/test.md");

			test_obj.Process("test", "test?param=1");
		}());
		auto resulting_document = buffer.str();
		REQUIRE_THAT(
		    resulting_document,
		    !Catch::Matchers::ContainsSubstring("%content%")
		);
		REQUIRE_THAT(
		    resulting_document,
		    !Catch::Matchers::ContainsSubstring("%title%")
		);
	}
}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
