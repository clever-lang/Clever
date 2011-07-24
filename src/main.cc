/**
 * Clever programming language
 * Copyright (c) 2011 Clever Team
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include "driver.h"
#ifdef _WIN32
#include "win32.h"
#endif

#define MORE_ARG() \
	if (!(++i < argc)) { \
		std::cerr << "Missing parameter for option " << argv[i-1] << std::endl; \
		exit(1); \
	}

static void show_usage(void) {
	std::cout << "Usage: clever <options>" << std::endl;
	std::cout << std::endl;

	std::cout << "General options:" << std::endl;
#ifdef _WIN32
	std::cout << "\t-b\tRun on background" << std::endl;
#endif
	std::cout << "\t-h\tHelp" << std::endl;
	std::cout << "\t-v\tShow version" << std::endl;
	std::cout << std::endl;

	std::cout << "Code options (must be the last one and unique):" << std::endl;
	std::cout << "\t-f\tExecute the file" << std::endl;
	std::cout << "\t-i\tRun the interative mode" << std::endl;
	std::cout << "\t-r\tRun the code" << std::endl;
	std::cout << "\t-qr\tQuickly run the code (import std automatically)" << std::endl; 
	std::cout << std::endl;

#ifdef CLEVER_DEBUG
	std::cout << "Debug options:" << std::endl;
	std::cout << "\t-p\tTrace parsing" << std::endl;
	std::cout << std::endl;
#endif
}

int main(int argc, char *argv[]) {
	clever::Interpreter clever;

	if (argc == 1) {
		std::cout << "Clever Programming Language" << std::endl;
		show_usage();
		return 0;
	}

	for (int i = 1; i < argc; ++i) {
		// Look for general options, then code options and finally debug options.
		if (argv[i] == std::string("-h")) {
			show_usage();
			return 0;
		} else if (argv[i] == std::string("-v")) {
			std::cout << "Clever - development version" << std::endl;
			return 0;
#ifdef _WIN32
		} else if (argv[i] == std::string("-b")) {
			if (GetConsoleWindow()) {
				std::string cline;

				for (int j = 0; j < argc; ++j) {
					if (argv[j] != std::string("-b")) {
						cline = cline + std::string(argv[j]) + " ";
					}
				}

				CreateBackgroundProcess(cline);
				return 0;
			}
#endif
		} else if (argv[i] == std::string("-f")) {
			MORE_ARG();
			clever.parseFile(argv[i]);
			break;
		} else if (argv[i] == std::string("-i")) {
			std::string input_line;

			while (std::cin) {
				getline(std::cin, input_line);
				clever.parseStr(input_line + '\n', false);
				clever.execute();
  			}
  			clever.shutdown();
  			return 0;
		} else if (argv[i] == std::string("-r")) {
			MORE_ARG();
			clever.parseStr(argv[i], false);
			break;
		} else if (argv[i] == std::string("-qr")) {
			MORE_ARG();
			clever.parseStr(argv[i], true);
			break;
#ifdef CLEVER_DEBUG
		} else if (argv[i] == std::string("-p")) {
			clever.trace_parsing = true;
#endif
		} else {
			std::cerr << "Unknown option '" << argv[i] << "'" << std::endl;
			exit(1);
		}
	}

	clever.execute();
	clever.shutdown();

	return 0;
}
