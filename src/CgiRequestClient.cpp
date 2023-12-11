/* CgiRequestClient.cpp
 * Copyright © 2023 Saul D. Beniuqez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "CgiRequestClient.hpp"
#include "IRouteHandler.hpp"
#include "exception.hpp"
#include "types.hpp"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

using namespace mdml;

#if not defined(TEST_SOCKET_PATH)
constexpr c::const_string SERVER_SOCKET_PATH = "/tmp/CgiRequestServer.sock";
#else
constexpr c::const_string SERVER_SOCKET_PATH = TEST_SOCKET_PATH;
#endif

constexpr size_t MAX_SOCKET_MSG_SIZE = 4 * 4096; // 16KiB

std::stringstream error_buffer;

CgiRequestClient::CgiRequestClient(int argc, c::const_string argv[],
                                   c::const_string env[])
    : Application(argc, argv, env),
      server_socket() {}
CgiRequestClient::~CgiRequestClient() {}

Result<std::string> CgiRequestClient::ProcessRequest() {
    auto server_response = std::string();
    try {
	std::stringstream buffer;
	auto &environment = Application::environment_variables;
	for (auto &[key, value] : environment) {
	    buffer << key << "=" << value << std::endl;
	}
	auto environment_blob = buffer.str();

	// Check if REQUEST_URI is in the environment map
	if (environment.find("REQUEST_URI") == environment.end()) {
	    throw mdml::exception("No REQUEST_URI in Environment!");
	}
	buffer.clear();
	buffer << environment.at("REQUEST_URI") << ";" << environment_blob
	       << std::flush;
	auto query_string = buffer.str();

	connect_to_server();
	server_response = query_server(query_string);

    } catch (const mdml::exception &except) {
	throw except;
    } catch (const std::exception &except) {
	throw mdml::exception(except);
    }

    return Result<std::string>{NO_ERROR, server_response};
}

const fs::path CgiRequestClient::SocketPath() {
    const static fs::path path(SERVER_SOCKET_PATH);

    return path;
};

inline void throw_socket_error(const std::string &msg) {
    error_buffer.clear();
    error_buffer << "{ msg: " << msg << ", socket_errno: " << errno << ", "
		 << "socket_err: \"" << strerror(errno)
		 << "\", socket_path: " << SERVER_SOCKET_PATH << "}"
		 << std::flush;
    throw mdml::exception(error_buffer.str().c_str());
}
void CgiRequestClient::connect_to_server() {
    auto &sockfd = this->server_socket;

    try {
	sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
	if (sockfd == -1) {
	    throw_socket_error("Error creating socket");
	}

	// Set up server address
	sockaddr_un server_address{0};

	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, SERVER_SOCKET_PATH);

	if (-1 == connect(sockfd, (struct sockaddr *)&server_address,
	                  sizeof(server_address))) {
	    throw_socket_error("Error connecting to server");
	}
    }

    catch (mdml::exception &e) {
	close(sockfd);
	throw;
    } catch (std::exception &e) {
	close(sockfd);
	throw mdml::exception(e);
    }
}

std::string CgiRequestClient::query_server(const std::string &query) {
    auto &sockfd = this->server_socket;
    auto socket_response = std::string();
    char socket_buffer[MAX_SOCKET_MSG_SIZE]{0};

    try {

	// Send the REQUEST_URI string
	if (send(sockfd, query.c_str(), query.size(), 0) == -1) {

	    throw_socket_error("Error sending data");
	}

	auto bytesRead =
	    recv(sockfd, socket_buffer, sizeof(socket_buffer) - 1, 0);
	if (bytesRead == -1) {
	    throw_socket_error("Error recieving data");
	} else {
	    socket_buffer[bytesRead] = '\0'; // Null-terminate the received data
	    socket_response = socket_buffer;
	}
    } catch (mdml::exception &e) {
	close(sockfd);
	throw;
    } catch (std::exception &e) {
	close(sockfd);
	throw;
    }

    close(sockfd);
    return socket_response;
}
//clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8
// sts=0 sw=8  noexpandtab ft=cpp.doxygen :
