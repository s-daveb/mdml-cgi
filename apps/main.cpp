/* main.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iomanip>
#include <iostream>
#include <map>
#include <string>

std::map<std::string, std::string> template_map;

void
load_template()
{
	template_map["index"] = L"<html>\
	<head><title>Hello world</title>\
	<meta charset=\"UTF-8\"></head>\
	<body>Testing 1 2 3 ☕️ </body>\
	</html>";
}

void
print_env(char**& envp)
{
	std::cout << "Env:" << std::endl;

	auto i = 0;
	for (auto** env = envp; *env != 0; env++) {
		auto thisEnv = std::string(*env);
		++i;
		std::cout << std::setw(24) << thisEnv << std::setw(0)
			  << std::flush;
		if ((i % 8) == 0) {
			std::cout << std::endl;
		} else {
			std::cout << " ";
		}
	}
}

int
main(int argc, char** argv, char** envp)
{
	load_template();
	std::cout << "Content-Type: text/plaintext" << std::endl << std::endl;

	print_env(envp);
	std::cout << std::endl;
	for (size_t index = 0; index < argc; ++index) {
		std::cout << argv[index] << " " << std::flush;
	}
	std::cout << std::endl;

	/*
	std::cout << "Content-Type: text/html" << std::endl << std::endl;
	std::cout << template_map["index"] << std::endl;
	*/
	return 0;
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
