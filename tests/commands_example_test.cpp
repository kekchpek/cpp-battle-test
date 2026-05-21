#include <App/Application.h>
#include <IO/System/EventLog.hpp>
#include <TestHelpers.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

namespace
{
	bool testScenarioEventLog(
		const char* testName,
		const std::string& scenarioRelativePath,
		const std::string& expectedRelativePath)
	{
		const std::string scenarioPath = std::string(TEST_DATA_DIR) + "/" + scenarioRelativePath;
		const std::string expectedPath = std::string(TEST_DATA_DIR) + "/" + expectedRelativePath;

		std::ifstream scenario(scenarioPath);
		if (!scenario)
		{
			std::cerr << testName << ": failed to open scenario: " << scenarioPath << '\n';
			return false;
		}

		std::ostringstream output;
		sw::EventLog eventLog(output);
		sw::core::app::Application application;
		application.run(scenario, eventLog);

		const auto actual = sw::test::splitNonEmptyLines(output.str());
		const auto expected = sw::test::readLines(expectedPath);

		return sw::test::compareLineByLine(actual, expected, testName);
	}
}

int main()
{
	if (!testScenarioEventLog(
			"commands_example_events",
			"tests/commands_example.txt",
			"tests/expected/commands_example_events.txt"))
	{
		return 1;
	}

	if (!testScenarioEventLog(
			"mutual_kill_events",
			"tests/mutual_kill.txt",
			"tests/expected/mutual_kill_events.txt"))
	{
		return 1;
	}

	std::cout << "commands_example_events: OK\n";
	std::cout << "mutual_kill_events: OK\n";
	return 0;
}
