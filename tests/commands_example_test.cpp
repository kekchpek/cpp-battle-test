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
	struct ScenarioTestCase
	{
		const char* name;
		const char* scenarioPath;
		const char* expectedPath;
	};

	bool testScenarioEventLog(const ScenarioTestCase& testCase)
	{
		const std::string scenarioPath = std::string(TEST_DATA_DIR) + "/" + testCase.scenarioPath;
		const std::string expectedPath = std::string(TEST_DATA_DIR) + "/" + testCase.expectedPath;

		std::ifstream scenario(scenarioPath);
		if (!scenario)
		{
			std::cerr << testCase.name << ": failed to open scenario: " << scenarioPath << '\n';
			return false;
		}

		std::ostringstream output;
		sw::EventLog eventLog(output);
		sw::core::app::Application application;
		application.run(scenario, eventLog);

		const auto actual = sw::test::splitNonEmptyLines(output.str());
		const auto expected = sw::test::readLines(expectedPath);

		return sw::test::compareLineByLine(actual, expected, testCase.name);
	}
}

int main()
{
	const ScenarioTestCase testCases[] = {
		{"commands_example_events", "tests/commands_example.txt", "tests/expected/commands_example_events.txt"},
		{"mutual_kill_events", "tests/mutual_kill.txt", "tests/expected/mutual_kill_events.txt"},
		{"small_map_duel_events", "tests/small_map_duel.txt", "tests/expected/small_map_duel_events.txt"},
		{"large_map_march_events", "tests/large_map_march.txt", "tests/expected/large_map_march_events.txt"},
		{"hunter_ranged_shot_events", "tests/hunter_ranged_shot.txt", "tests/expected/hunter_ranged_shot_events.txt"},
		{"hunter_shadow_strike_events", "tests/hunter_shadow_strike.txt", "tests/expected/hunter_shadow_strike_events.txt"},
		{"single_unit_events", "tests/single_unit.txt", "tests/expected/single_unit_events.txt"},
		{"two_pairs_events", "tests/two_pairs.txt", "tests/expected/two_pairs_events.txt"},
		{"duel_and_ranged_events", "tests/duel_and_ranged.txt", "tests/expected/duel_and_ranged_events.txt"},
	};

	for (const auto& testCase : testCases)
	{
		if (!testScenarioEventLog(testCase))
		{
			return 1;
		}

		std::cout << testCase.name << ": OK\n";
	}

	return 0;
}
