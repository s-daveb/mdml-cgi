/* main.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Application.hpp"

#include <iomanip>
#include <iostream>
#include <map>
#include <string>

int
main(int argc, char** argv, char** envp)
{
	mdml::Application app(argc, argv, envp);
	for (int i = 0; i < argc; ++i) {
		std::cout << argv[i] << std::endl;
	}
	return 0;
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
