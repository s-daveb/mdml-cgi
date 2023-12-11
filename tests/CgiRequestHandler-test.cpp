/* CgiRequestHandler-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "CgiRequestHandler.hpp"
#include "IRouteHandler.hpp"
#include "exception.hpp"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "tests-common.hpp"
#include <catch2/matchers/catch_matchers_string.hpp>

namespace {
using namespace mdml;

struct simulated_launch {
    static const char *argv[];
    static const char *env[];
};
inline const char *simulated_launch::argv[] = {"param1", "param2", "param3"};
inline const char *simulated_launch::env[] = {
    "PATH=/usr/bin", "VAR2=TWO", "REQUEST_URI=markdown?msg=hello-world",
    nullptr};

// Define a test oute handler for the "/markdown" route
class ExampleRouteHandler : public mdml::IRouteHandler {
  public:
    virtual mdml::Result<std::string>
    Process(const std::string &name, const std::string &request_uri) override {
	// Implement test logic for the route
	// handler
	return {mdml::NO_ERROR, "Processed"};
    }
    virtual ~ExampleRouteHandler() = default;
};
} // namespace
// @{ test_client
struct test_client {
    test_client() : is_running(false), server_socket(-1), socket_address({0}) {}

    ~test_client() {
	is_running = false;
	close(server_socket);
	unlink(socket_address.sun_path);
    }

    void connect() {
	auto &sockfd = this->server_socket;
	server_socket = socket(PF_UNIX, SOCK_STREAM, 0);

	if (server_socket == -1) {
	    throw mdml::exception("Error creating socket");
	}

	socket_address.sun_family = AF_UNIX;
	strcpy(socket_address.sun_path, TEST_SOCKET_PATH);
	unlink(socket_address.sun_path);
	if (-1 == ::connect(sockfd, (struct sockaddr *)&socket_address,
	                    sizeof(socket_address))) {
	    throw mdml::exception("Error connecting to server");
	}
    }

    auto send() -> void {
	/*
	// Listen for incoming connections
	if (listen(server_socket, SOMAXCONN) == -1) {
	    throw mdml::exception("Error listening on socket");
	}

	server_listening = true;

	int client_socket = accept(server_socket, nullptr, nullptr);
	if (client_socket == -1) {
	    throw mdml::exception("Error accepting connection");
	}

	auto bytes_written =
	    write(client_socket, TEST_SOCK_MESSAGE, strlen(TEST_SOCK_MESSAGE));
	*/
    }

    fd::socket server_socket;
    sockaddr_un socket_address;
    bool is_running;
};

BEGIN_TEST_SUITE("CgiRequestHandler") {
    using namespace mdml;
    struct TestFixture {
	TestFixture() : cgi_app(), request_info({0}) {}

	CgiRequestHandler cgi_app;
	RequestInfo request_info;
    };

    TEST("CgiRequestHandler Constructor Test") { CgiRequestHandler test; };

    TEST("CgiRequestHandler ImportRoutes Test") {

	CgiRequestHandler test;

	Dictionary<IRouteHandler_ptr> routes;
	routes.emplace("test", new ExampleRouteHandler());
	routes.emplace("test2", new ExampleRouteHandler());
	routes.emplace("fasdfdshjk", new ExampleRouteHandler());

	test.ImportRoutes(routes);

	REQUIRE(routes.size() < 1);

	REQUIRE(test.routes.size() == 3);
	REQUIRE(test.routes.at("test") != nullptr);
    };

    TEST("CgiRequestHandler::ProcessRequest no REQUEST_URI variable") {
	const char *no_request_env[] = {"PATH=/usr/bin", "VAR2=TWO", nullptr};
	CgiRequestHandler test;
	auto request_info = RequestInfo{0};
	REQUIRE_THROWS_AS([&]() { test.ProcessRequest(request_info); }(),
	                  mdml::exception);
    }

    FIXTURE_TEST("CgiRequestHandler ProcessRequest with valid route") {

	// Add the test route handler to the Routes dictionary
	auto handler = mdml::managed::make_ptr<ExampleRouteHandler>();

	cgi_app.routes["markdown"] = handler;

	auto result = cgi_app.ProcessRequest(request_info);

	// Add assertions to check the result
	CHECK(result.ErrorData == "Processed");
	REQUIRE(result.IsError == false);
    }

    FIXTURE_TEST("CgiRequestHandler ProcessRequest with unknown route") {

	// Remove the "markdown" route from the Routes
	// dictionary
	for (auto &[key, value] : cgi_app.routes) {
	    if (value.get() != nullptr) {
		value.reset();
	    }
	    cgi_app.routes.erase(key);
	}

	auto result = cgi_app.ProcessRequest(request_info);

	// Add assertions to check the result
	CHECK(result.ErrorData != "Success");
	REQUIRE(result.IsError == true);
    }

    FIXTURE_TEST("CgiRequestHandler ProcessRequest Route throws exception") {
	// Define a test oute handler for the "/markdown" route
	class BadRouteHandler : public IRouteHandler {
	  public:
	    virtual Result<std::string>
	    Process(const std::string &name,
	            const std::string &request_uri) override {
		throw std::runtime_error("ERROR");
		return {mdml::ERROR, "This line never executes"};
	    }
	    virtual ~BadRouteHandler() = default;
	};
	auto handler = mdml::managed::make_ptr<BadRouteHandler>();
	cgi_app.routes["markdown"] = handler;

	REQUIRE_THROWS_AS([&]() { cgi_app.ProcessRequest(request_info); }(),
	                  mdml::exception);
    }
}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
