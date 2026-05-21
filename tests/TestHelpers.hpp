#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace sw::test
{
	inline std::vector<std::string> readLines(const std::string& path)
	{
		std::ifstream file(path);
		if (!file)
		{
			throw std::runtime_error("Failed to open file: " + path);
		}

		std::vector<std::string> lines;
		std::string line;
		while (std::getline(file, line))
		{
			if (!line.empty() && line.back() == '\r')
			{
				line.pop_back();
			}
			lines.push_back(line);
		}
		return lines;
	}

	inline bool compareLineByLine(
		const std::vector<std::string>& actual,
		const std::vector<std::string>& expected,
		const char* testName)
	{
		if (actual.size() != expected.size())
		{
			std::cerr << testName << ": line count mismatch, expected " << expected.size()
					  << ", got " << actual.size() << '\n';
			return false;
		}

		bool ok = true;
		for (std::size_t i = 0; i < expected.size(); ++i)
		{
			if (actual[i] != expected[i])
			{
				std::cerr << testName << ": mismatch at line " << (i + 1) << '\n'
						  << "  expected: " << expected[i] << '\n'
						  << "  actual:   " << actual[i] << '\n';
				ok = false;
			}
		}
		return ok;
	}

	inline std::vector<std::string> splitNonEmptyLines(const std::string& text)
	{
		std::vector<std::string> lines;
		std::istringstream stream(text);
		std::string line;
		while (std::getline(stream, line))
		{
			if (!line.empty())
			{
				lines.push_back(line);
			}
		}
		return lines;
	}
}
