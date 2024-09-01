#pragma once

#include <cstdint>
#include <vector>

#include <base/iMemory.hpp>

class MemoryWrapper: public IMemory
{
public:
	MemoryWrapper(std::vector<uint8_t> raw_memory): _memory(std::move(raw_memory)) {}
	virtual ~MemoryWrapper() = default;

	uint8_t read(const uint16_t address) const override { return this->_memory[address]; }
	void write(const uint16_t address, const uint8_t data) override
	{
		this->_memory[address] = data;
	}
	uint16_t size() { return this->_memory.size(); }

private:
	std::vector<uint8_t> _memory;
};
