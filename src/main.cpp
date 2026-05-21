#include <App/Application.h>
#include <IO/System/EventLog.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

	sw::EventLog eventLog;
	sw::core::app::Application application;
	application.run(file, eventLog);

	return 0;
}
