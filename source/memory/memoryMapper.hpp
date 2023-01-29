#pragma once

#include "iMemory.hpp"
#include "randomAccessMemory.hpp"

class MemoryMapper: public IMemory
{
public:
	MemoryMapper();
	virtual ~MemoryMapper();

	uint8_t read(uint16_t address) const override;
	void write(uint16_t address, const uint8_t data) override;

private:
	RandomAccessMemory* _ram {nullptr};
};
