#include <vector>

#include "iMemory.hpp"

class SlowMemory: public IMemory
{
public:
	SlowMemory(const uint32_t size = 0x10000);
	virtual ~SlowMemory() = default;

	uint8_t read(uint16_t address) const override { return this->_data[address]; }
	void write(const uint16_t address, const uint8_t data) override { this->_data[address] = data; }

	std::vector<uint8_t> _data;
};
