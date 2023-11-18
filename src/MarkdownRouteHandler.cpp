/* MarkdownRouteHandler.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Application.hpp"
#include "IRouteHandler.hpp"
#include "MarkdownRouteHandler.hpp"
#include "exception.hpp"
#include "types.hpp"

#include "cmark.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace mdml;

#if defined(TESTING)
constexpr const char* DEFAULT_WORKDIR = ".";
#else
constexpr const char* DEFAULT_WORKDIR = "/usr/local/www/templates";
#endif

const auto NOT_FOUND = std::string::npos;

void
string_replace(
    std::string& out_buffer, const std::string& pattern,
    const std::string& replacement
)
{
	for (auto pattern_pos = out_buffer.find(pattern);
	     pattern_pos != NOT_FOUND;
	     pattern_pos = out_buffer.find(pattern)) {
		if (pattern_pos != NOT_FOUND) {
			out_buffer.replace(
			    pattern_pos, pattern.length(), replacement
			);
		}
	}
}

MarkdownRouteHandler::MarkdownRouteHandler()
    : IRouteHandler(), OutputStream(std::cout), work_dir(DEFAULT_WORKDIR)
{
	work_dir = fs::absolute(work_dir);
}

MarkdownRouteHandler::~MarkdownRouteHandler() {}

void
MarkdownRouteHandler::LoadTemplate(const std::string& template_filename)
{
	fs::path full_html_path = work_dir / template_filename;

	this->load_document(full_html_path, this->html_data);
}

void
MarkdownRouteHandler::LoadMarkdown(const std::string& markdown_filename)
{
	fs::path full_md_path = work_dir / markdown_filename;

	this->load_document(full_md_path, this->markdown_data);
}

Result<std::string>
MarkdownRouteHandler::Process(
    const std::string& name, const std::string& request_uri
)
{
	auto document = render_document(name, request_uri);
	auto& out = OutputStream.get();

	out << document << std::flush;

	return { NO_ERROR, document };
}

void
MarkdownRouteHandler::load_document(
    const fs::path& document_path, std::string& out_document
)
{
	if (!fs::exists(document_path) || !fs::is_regular_file(document_path)) {
		auto error = "File not found: " + document_path.string();
		throw mdml::exception(error);
	}

	try {
		std::ifstream file(document_path);
		if (file.is_open()) {
			std::string htmlContent(
			    (std::istreambuf_iterator<char>(file)),
			    std::istreambuf_iterator<char>()
			);
			out_document = htmlContent;
		} else {
			auto error_message =
			    (std::stringstream()
			     << "Could not open file '"
			     << document_path.filename() << "', at path  "
			     << document_path.parent_path() << ". ")
				.str();
			throw mdml::exception(error_message);
		}
	} catch (const mdml::exception& e) {
		throw e;
	} catch (const std::exception& e) {
		throw mdml::exception(e);
	}
}

std::string
MarkdownRouteHandler::render_document(
    const std::string& title, const std::string& request_uri
)
{
	std::string result = this->html_data;
	std::string token = "%content%";

	std::string generated_html = cmark_markdown_to_html(
	    this->markdown_data.c_str(),
	    this->markdown_data.length(),
	    CMARK_OPT_DEFAULT
	);

	string_replace(result, token, generated_html);
	string_replace(result, "%title%", title);

	return result;
}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
