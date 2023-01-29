#pragma once

#include "iMemory.hpp"

class RandomAccessMemory: public IMemory
{
public:
	RandomAccessMemory() = delete;
	RandomAccessMemory(uint16_t size);
	virtual ~RandomAccessMemory();

	uint8_t read(uint16_t address) const override { return this->_data[address]; }
	void write(uint16_t address, const uint8_t data) override { this->_data[address] = data; }

private:
	uint8_t* _data = nullptr;
};
