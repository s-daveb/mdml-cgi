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
#include <iostream>
#include <optional>

BEGIN_TEST_SUITE("exceptions")
{
	// @todo: move this to a runtime-test.cpp file
	TEST("Test the test framework")
	{
		REQUIRE_THROWS([]() {
			throw std::runtime_error("Error Message");
		}());

		try {
			throw mdml::exception(std::runtime_error("ERROR"));
		} catch (mdml::exception& e) {
			std::cout << "Caught exception: " << e.what()
				  << std::endl;
		}

	} // #endregion
}
