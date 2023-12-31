/* Application-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "exception.hpp"
#include "tests-common.hpp"

#include "Application.hpp"

#include <functional>
#include <optional>

struct simulated_launch {
	static const char* argv[];
	static const char* env[];
};
inline const char* simulated_launch::argv[] = { "param1", "param2", "param3" };
inline const char* simulated_launch::env[] = { "PATH=/usr/bin",
	                                       "VAR2=TWO",
	                                       nullptr };

template<typename T>
using opt_reference = std::optional<std::reference_wrapper<T>>;

BEGIN_TEST_SUITE("Application-test")
{
	TEST("Application Class construction")
	{
		auto simple_construction = []() {
			auto test_object = mdml::Application(
			    3, simulated_launch::argv, simulated_launch::env
			);
		};
		auto double_construction = []() {
			auto test_object = mdml::Application(
			    3, simulated_launch::argv, simulated_launch::env
			);
			auto throws_errors = mdml::Application(
			    3, simulated_launch::argv, simulated_launch::env
			);
		};

		SECTION("First construction")
		{
			REQUIRE_NOTHROW(simple_construction());
		}
		SECTION("Double construction")
		{
			REQUIRE_THROWS_AS(
			    double_construction(), mdml::exception
			);
		}
		SECTION("Get existing instance")
		{
			opt_reference<mdml::Application> app;
			try {
				auto test_object = mdml::Application(
				    3,
				    simulated_launch::argv,
				    simulated_launch::env
				);
				app = test_object;
			} catch (const mdml::exception& e) {
				app = mdml::Application::GetInstance();
			}
		}
		auto incorrect_construction = []() {
			mdml::Application obj(0, nullptr, nullptr);
		};

		SECTION("Incorrect parameters")
		{
			REQUIRE_THROWS_AS(
			    incorrect_construction(), mdml::exception
			);
		}
	}

	TEST("Application Parameter capture")
	{
		SECTION("Arguments are captured in vector")
		{
			auto correct_construction = []() {
				auto test_object = mdml::Application(
				    3,
				    simulated_launch::argv,
				    simulated_launch::env
				);
				auto args_list = test_object.Arguments();

				SECTION("Argument verification")
				{
					REQUIRE(args_list[0] == "param1");
					REQUIRE(args_list[1] == "param2");
				}
			};
			REQUIRE_NOTHROW(correct_construction());
		}
		SECTION("Environemnt variables are catpured in map")
		{
			auto correct_construction = []() {
				auto test_object = mdml::Application(
				    3,
				    simulated_launch::argv,
				    simulated_launch::env
				);
				auto environ = test_object.Environment();

				SECTION("Varaible verification")
				{
					REQUIRE(environ["PATH"] == "/usr/bin");
					REQUIRE(environ["VAR2"] == "TWO");
				}
			};
			REQUIRE_NOTHROW(correct_construction());
		}
	}
}

// clang-format off
// vim: set foldmethod=syntax foldlevel=1000 textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
