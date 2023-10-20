/* exceptions-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "exception.hpp"

#include "tests-common.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>

namespace Match = Catch::Matchers;
using Catch::CaseSensitive;

void
throw_local_variable()
{
	auto local = std::logic_error("This is a stack variable");

	// Stack variables should not be used outside their stack frames.
	throw mdml::exception(local);
}

BEGIN_TEST_SUITE("mdml::exception")
{
	auto throw_an_exception = []() {
		throw mdml::exception("An error occurred!!");
	};

	TEST("TEST: Can throw new exception type")
	{
		REQUIRE_THROWS(throw_an_exception());
	}

	TEST("TEST: Can construct exception various ways")
	{
		SECTION("1. Blank constructor")
		{
			mdml::exception obj;
		}
		SECTION("2. With string parameter")
		{
			mdml::exception obj("Sample Error");
		}
		SECTION("3. With STL exception")
		{
			mdml::exception obj(std::runtime_error("Sample Error"));
		}
		SECTION("4. With destroyed stack")
		{
			auto nested_function_call = []() {
				throw_local_variable();
			};
			try {
				nested_function_call();
			} catch (std::exception& e) {
				REQUIRE_THAT(
				    e.what(),
				    Match::ContainsSubstring(
					"This is a stack variable"
				    )
				);
			}
		}
	}

	TEST("TEST: what() message reflects error")
	{

		SECTION("1. Unspecified error or exception")
		{
			mdml::exception obj;

			REQUIRE_THAT(
			    obj.what(),
			    Match::ContainsSubstring(
				mdml::exception::default_error,
				CaseSensitive::Yes
			    )
			);
		}

		SECTION("2. custom error or exception")
		{
			constexpr auto test_message = "This is a test.";
			mdml::exception test_object_one(test_message);
			mdml::exception test_object_two(
			    std::logic_error("Makes no sense")
			);
			SECTION(" a: what() does not contain default message")
			{
				REQUIRE_THAT(
				    test_object_one.what(),
				    !Match::ContainsSubstring(
					mdml::exception::default_error
				    )
				);
			}
			SECTION(" b: what() displays custom message")
			{
				REQUIRE_THAT(
				    test_object_one.what(),
				    Match::ContainsSubstring(test_message)
				);
			}
			SECTION(" c: what() contains inner exception message")
			{
				REQUIRE_THAT(
				    test_object_two.what(),
				    Match::ContainsSubstring("Makes no sense")
				);
			}
		}
	}

	TEST("TEST: what() contains stacktrace with Catch2 runtime methods")
	{
		mdml::exception test_object("Test");
		SECTION(" a: what() does not contain default message")
		{
			REQUIRE_THAT(
			    test_object.what(),
			    Match::ContainsSubstring("Catch::RunContext")
			);
			SUCCEED(test_object.what());
		}
	}
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=4 sts=0 sw=4  noexpandtab ft=cpp.doxygen :
