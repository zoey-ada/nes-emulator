#pragma once

#include "iMemory.hpp"

class Cartridge;
class RandomAccessMemory;

class PpuMemoryMapper: public IMemory
{
public:
	PpuMemoryMapper();
	virtual ~PpuMemoryMapper();

	uint8_t read(uint16_t address) const override;
	void write(uint16_t address, const uint8_t data) override;

	void load_cartridge(Cartridge* cartridge);
	void unload_cartridge();

private:
	RandomAccessMemory* _ram {nullptr};
	Cartridge* _cartridge;
};
