#include <vector>

#include <cpu/iMemory.hpp>

class MockMemory: public IMemory
{
public:
	MockMemory();
	virtual ~MockMemory() = default;

	uint8_t read(uint16_t address) const override { return this->_data[address]; }
	void write(const uint16_t address, const uint8_t data) override { this->_data[address] = data; }

	std::vector<uint8_t> _data;
};
