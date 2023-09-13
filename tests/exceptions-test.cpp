/* exceptions-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "exception.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>

#define TEST(testname) TEST_CASE(testname, TEST_SUITE_NAME)

namespace {
const char* TEST_SUITE_NAME = "mdml::exceptions";

// @todo: move this to a runtime-test.cpp file
TEST("Test the test framework")
{
	REQUIRE_THROWS(
	    []() { throw std::runtime_error("Error Message"); }(),
	    "Expected Error Message"
	);

	try {
		throw mdml::exception(std::runtime_error("NOT IMPLEMENTED"));
	} catch (mdml::exception& e) {
		std::cout << e.what() << std::endl;
	}

} // #endregion

}
