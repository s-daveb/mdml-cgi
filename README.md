# mdml-cgi

`mdml-cgi` is a C++ application designed to be used as a Common Gateway Interface (CGI) script by web servers. It facilitates the generation of HTML content by combining a template and a Markdown file. The project utilizes the [cmark](https://github.com/commonmark/cmark) library for Markdown parsing. The license for the cmark library can be found in the root directory.

## Features

- **Markdown to HTML:** Convert Markdown content to HTML using the powerful cmark library.
- **Template Support:** Generate HTML by combining a template with the parsed Markdown content.
- **CGI Integration:** Seamless integration with web servers as a CGI script for dynamic content generation.

## Dependencies

- [cmark](https://github.com/commonmark/cmark): A C library for parsing CommonMark Markdown documents.
  - License: [cmark License](path/to/cmark/license/file)
- [Catch2](https://github.com/catchorg/Catch2): A modern, C++-native, header-only framework for unit testing.
  - License: [Catch2 License](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt)

## Building and Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/your-username/mdml-cgi.git
   
2. **Build the project:**
   ```bash
   cmake -B release -DCMAKE_BUILD_TYPE=Release 
   make -C release 
   ```
   or 
   ```bash
   cmake -B release -DCMAKE_BUILD_TYPE=Release  -G Ninja
   ninja -C release
   ```

## Testing

Unit tests for `mdml-cgi` are implemented using the Catch2 framework. To run the tests, execute:

```bash
cmake -B debug -DCMAKE_BUILD_TYPE=Debug 
 make -C debug ctest
```
or 
```bash
cmake -B debug -DCMAKE_BUILD_TYPE=Debug  -G ninja
ninja -C debug ctest
```
## Usage

To use `mdml-cgi` as a CGI script, follow these steps:

1. Configure your web server to recognize `mdml-cgi` as a CGI script.
2. Ensure the required dependencies are installed.
3. Place your Markdown file and template in a designated directory.
4. Make a request to the CGI script, passing the Markdown file and template as parameters.

## License

This project is licensed under the [Mozilla Public License](LICENSE).

## Contributing

Please feel free to submit pull requests, but this project is a small personal project for now, that I'm using to build a portfolio and expose myself to modern C++.
