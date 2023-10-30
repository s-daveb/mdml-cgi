/* Result-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "tests-common.hpp"

#include "types.hpp"
#include "types/Result.hpp"

BEGIN_TEST_SUITE("mdml::Result")
{
	TEST("Construction of Result object")
	{
		SECTION("Using bool to initialize")
		{
			REQUIRE_NOTHROW([]() {
				mdml::Result test{ false, "Success" };
			}());
		}

		SECTION("Using error_t to initialize")
		{
			REQUIRE_NOTHROW([]() {
				mdml::Result test{ mdml::NO_ERROR, "Success" };
			}());
		}
	}

	TEST("Assignment of Result Object")
	{
		SECTION("Copy Constructor")
		{
			auto test_steps = []() {
				mdml::Result obj1{ false, "Success" };
				auto obj2(obj1);

				CHECK(obj1.IsError == obj2.IsError);
				CHECK(obj1.ErrorData == obj2.ErrorData);
			};
			REQUIRE_NOTHROW(test_steps());
		}

		SECTION("Operator=() call")
		{
			auto test_steps = []() {
				mdml::Result obj1{ false, "Success" };
				auto obj2 = obj1;

				CHECK(obj1.IsError == obj2.IsError);
				CHECK(obj1.ErrorData == obj2.ErrorData);
			};
			REQUIRE_NOTHROW(test_steps());
		}
	}
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
