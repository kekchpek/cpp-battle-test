#include <App/Application.h>
#include <IO/System/EventLog.hpp>
#include <TestHelpers.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

namespace
{
	const char* kTestName = "commands_example_events";

	bool testCommandsExampleEventLog()
	{
		const std::string scenarioPath = std::string(TEST_DATA_DIR) + "/commands_example.txt";
		const std::string expectedPath =
			std::string(TEST_DATA_DIR) + "/tests/expected/commands_example_events.txt";

		std::ifstream scenario(scenarioPath);
		if (!scenario)
		{
			std::cerr << kTestName << ": failed to open scenario: " << scenarioPath << '\n';
			return false;
		}

		std::ostringstream output;
		sw::EventLog eventLog(output);
		sw::core::app::Application application;
		application.run(scenario, eventLog);

		const auto actual = sw::test::splitNonEmptyLines(output.str());
		const auto expected = sw::test::readLines(expectedPath);

		return sw::test::compareLineByLine(actual, expected, kTestName);
	}
}

int main()
{
	if (!testCommandsExampleEventLog())
	{
		return 1;
	}

	std::cout << kTestName << ": OK\n";
	return 0;
}
