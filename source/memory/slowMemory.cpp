#include "slowMemory.hpp"

SlowMemory::SlowMemory(const uint16_t size)
{
	this->_data = std::vector<uint8_t>(size, 0);
}
