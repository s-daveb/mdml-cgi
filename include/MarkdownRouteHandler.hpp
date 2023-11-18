/* MarkdownRouteHandler.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "IRouteHandler.hpp"
#include "types.hpp"

#include <filesystem>
#include <string>

namespace mdml {

namespace {
namespace fs = std::filesystem;
}

class MarkdownRouteHandler : public IRouteHandler {
    public:
	MarkdownRouteHandler();
	virtual ~MarkdownRouteHandler();

	void LoadTemplate(const std::string& template_name);
	void LoadMarkdown(const std::string& markdown_page_name);

	virtual Result<std::string> Process(
	    const std::string& name, const std::string& request_uri
	);

	std::reference_wrapper<std::ostream> OutputStream;

#ifdef TESTING
	inline std::string& GetHtmlData() { return html_data; }
	inline std::string& GetMarkdownData() { return markdown_data; }
#endif
    protected:
	std::string render_document(
	    const std::string& title, const std::string& request_uri
	);

	static void load_document(
	    const fs::path& document_path, std::string& out_document
	);

	fs::path work_dir;
	std::string html_data;
	std::string markdown_data;
};

}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
