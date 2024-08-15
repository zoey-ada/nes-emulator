#pragma once

#include <memory>

#include "iMemory.hpp"

class Cartridge;
class PictureProcessingUnit;

class MemoryMapper: public IMemory
{
public:
	MemoryMapper(PictureProcessingUnit* ppu);
	virtual ~MemoryMapper();

	uint8_t read(uint16_t address) const override;
	void write(uint16_t address, const uint8_t data) override;

	void load_cartridge(Cartridge* cartridge);
	void unload_cartridge();

private:
	std::unique_ptr<IMemory> _ram {nullptr};
	Cartridge* _cartridge {nullptr};
	PictureProcessingUnit* _ppu {nullptr};
};
