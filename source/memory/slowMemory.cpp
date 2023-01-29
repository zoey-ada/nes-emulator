#include "slowMemory.hpp"

SlowMemory::SlowMemory()
{
	this->_data = std::vector<uint8_t>(0x10000, 0);
}
