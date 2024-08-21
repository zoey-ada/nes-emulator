#include "slowMemory.hpp"

SlowMemory::SlowMemory(const uint32_t size)
{
	this->_data = std::vector<uint8_t>(size, 0);
}

void SlowMemory::dumpMemory(std::ostream& outstream)
{
	for (const auto& byte : this->_data)
		outstream << byte;
}
