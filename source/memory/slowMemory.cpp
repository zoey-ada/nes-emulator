#include "slowMemory.hpp"

SlowMemory::SlowMemory(const uint32_t size)
{
	this->_data = std::vector<uint8_t>(size, 0);
}
