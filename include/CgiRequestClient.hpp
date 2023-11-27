/* CgiRequestClient.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Application.hpp"
#include "types.hpp"

#include <memory>
#include <string>
#include <vector>

namespace mdml {

#ifdef TESTING
constexpr c::const_string TEST_SOCKET_PATH = "/tmp/cgi-req-test.sock";
#endif
class IRouteHandler;

class CgiRequestClient : public Application {

    public:
	CgiRequestClient(
	    int argc, c::const_string argv[], c::const_string env[]
	);
	virtual ~CgiRequestClient();
	Result<std::string> ProcessRequest();

    private:
	void connect_to_server();
	std::string query_server(const std::string& query);
	fd::socket server_socket;
};
}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=81 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
