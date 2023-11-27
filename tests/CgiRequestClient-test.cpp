/* CgiRequestClient-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "CgiRequestClient.hpp"
#include "exception.hpp"

#include "tests-common.hpp"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <functional>
#include <list>
#include <thread>

namespace {
struct simulated_launch {
	static const char* argv[];
	static const char* env[];
};
inline const char* simulated_launch::argv[] = { "param1", "param2", "param3" };
inline const char* simulated_launch::env[] = {
	"PATH=/usr/bin",
	"VAR2=TWO",
	"REQUEST_URI=markdown?msg=hello-world",
	nullptr
};
}

using namespace mdml;

bool server_listening = false;
constexpr c::const_string TEST_SOCK_MESSAGE = "🧦";

// @{ test_server
struct test_server {
	test_server() : is_running(false), server_socket(-1), socket_addr({ 0 })
	{
	}

	~test_server()
	{
		is_running = false;
		close(server_socket);
		unlink(socket_addr.sun_path);
	}

	void bind()
	{
		server_socket = socket(PF_UNIX, SOCK_STREAM, 0);
		if (server_socket == -1) {
			throw mdml::exception("Error creating socket");
		}

		// Set up the sockaddr_un structure
		socket_addr.sun_family = AF_UNIX;
		strcpy(socket_addr.sun_path, mdml::TEST_SOCKET_PATH);

		unlink(socket_addr.sun_path);
		if (::bind(
			server_socket,
			(sockaddr*)&socket_addr,
			sizeof(socket_addr)
		    ) == -1) {
			throw mdml::exception("Error binding socket");
		}
	}

	auto listen_and_accept() -> void
	{
		// Listen for incoming connections
		if (listen(server_socket, SOMAXCONN) == -1) {
			throw mdml::exception("Error listening on socket");
		}

		server_listening = true;

		int client_socket = accept(server_socket, nullptr, nullptr);
		if (client_socket == -1) {
			throw mdml::exception("Error accepting connection");
		}

		auto bytes_written = write(
		    client_socket, TEST_SOCK_MESSAGE, strlen(TEST_SOCK_MESSAGE)
		);
	}

	fd::socket server_socket;
	sockaddr_un socket_addr;
	bool is_running;
};

auto
server_thread() -> void
{
	test_server server;
	server.bind();
	server.listen_and_accept();
} // @}

BEGIN_TEST_SUITE("CgiRequestClient")
{
	using namespace mdml;
	struct TestFixture {
		const char* envp[4] = { "PATH=/usr/bin",
			                "VAR2=TWO",
			                "REQUEST_URI=markdown?msg=hello-world",
			                nullptr };
		TestFixture() : client_app(1, simulated_launch::argv, envp) {}

		CgiRequestClient client_app;
	};

	TEST("CgiRequestClient Constructor Test")
	{

		CgiRequestClient test(
		    1, simulated_launch::argv, simulated_launch::env
		);
	};

	TEST("CgiRequestClient::ProcessRequest no REQUEST_URI variable")
	{
		const char* no_request_env[] = { "PATH=/usr/bin",
			                         "VAR2=TWO",
			                         nullptr };
		CgiRequestClient test(1, simulated_launch::argv, no_request_env);
		REQUIRE_THROWS_AS(
		    [&]() { test.ProcessRequest(); }(), mdml::exception
		);
	}

	FIXTURE_TEST("CgiRequestClient ProcessRequest with missing socket")
	{

		REQUIRE_THROWS([&]() {
			auto result = client_app.ProcessRequest();

			// Add assertions to check the result
			CHECK(result.ErrorData != "Success!");
			REQUIRE(result.IsError != false);
		}());
	}

	FIXTURE_TEST("CgiRequestClient ProcessRequest with socket error")
	{

		// spawn a thread that listens on a socket
		REQUIRE_THROWS([&]() {
			auto result = client_app.ProcessRequest();

			// Add assertions to check the result
			CHECK(result.ErrorData == "Processed");
			REQUIRE(result.IsError == false);
		}());
	}

	FIXTURE_TEST("CgiRequestClient ProcessRequest Success")
	{
		std::list<std::thread> threads;

		try {
			threads.push_back(std::thread(server_thread));

			while (!server_listening) {
				; // Wait for server to initialize
			}

			auto server_response = client_app.ProcessRequest();

			REQUIRE(server_response.IsError == false);
			REQUIRE(server_response.ErrorData == TEST_SOCK_MESSAGE);

			for (auto& thread : threads) {
				thread.join();
			}

		} catch (mdml::exception& e) {
			for (auto& thread : threads) {
				thread.detach();
			}
			throw;
		} catch (std::exception& e) {
			for (auto& thread : threads) {
				thread.detach();
			}
			throw;
		}
	}
}

// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
