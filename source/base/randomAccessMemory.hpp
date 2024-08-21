#pragma once

#include <cstdint>
#include <ostream>

#include "iMemory.hpp"

class RandomAccessMemory: public IMemory
{
public:
	RandomAccessMemory(uint16_t size);
	virtual ~RandomAccessMemory();

	uint8_t read(uint16_t address) const override { return this->_data[address]; }
	void write(uint16_t address, const uint8_t data) override { this->_data[address] = data; }
	void dumpMemory(std::ostream& outstream);

private:
	uint8_t* _data = nullptr;
	uint64_t _size {0};
};
