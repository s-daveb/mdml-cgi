/* exceptions-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <catch2/catch_test_macros.hpp>

#define TEST(testname) TEST_CASE(#testname, TEST_SUITE_NAME)

namespace {
	const char* TEST_SUITE_NAME = "[class:Observable]";

	TEST("Register IObservers")
	{
		REQUIRE( true == false );
	} // #endregion
}
