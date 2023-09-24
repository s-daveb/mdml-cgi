/* main.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <map>
#include <string>

std::map<std::string, std::wstring> template_map;

void
load_template()
{
	template_map["index"] = "<html>\
	<head><title>Hello world</title>\
	<meta charset=\"UTF-8\"></head>\
	<body>Testing 1 2 3 ☕️ </body>\
	</html>";
}

int
main(int argc, char** argv)
{
	load_template();

	std::cout << "Content-Type: text/html" << std::endl << std::endl;
	std::wcout << template_map["index"] << std::endl;
	return 0;
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
