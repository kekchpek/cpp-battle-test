#pragma once

#include "details/PrintFieldVisitor.hpp"
#include <iostream>
#include <typeindex>
#include <unordered_map>

namespace sw
{
	class EventLog
	{
	private:
		std::ostream& _stream;
		uint64_t _tick{1};
	public:
		explicit EventLog(std::ostream& stream = std::cout) :
				_stream(stream)
		{}

		template <class TEvent>
		void log(TEvent&& event)
		{
			_stream << "[" << _tick << "] " << TEvent::Name << " ";
			PrintFieldVisitor visitor(_stream);
			event.visit(visitor);
			_stream << std::endl;
		}

		void incrementTick()
		{
			_tick++;
		}
	};
}
