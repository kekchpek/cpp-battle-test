#pragma once

#include <string>

namespace sw::io
{
	struct ErrorOccurred
	{
		constexpr static const char* Name = "ERROR_OCCURRED";

		std::string message{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("message", message);
		}
	};
}
