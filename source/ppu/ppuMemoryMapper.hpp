#pragma once

#include <memory>

#include <base/iMemory.hpp>

class Cartridge;

class PpuMemoryMapper: public IMemory
{
public:
	PpuMemoryMapper();
	virtual ~PpuMemoryMapper();

	uint8_t read(uint16_t address) const override;
	void write(uint16_t address, const uint8_t data) override;

	void load_cartridge(Cartridge* cartridge);
	void unload_cartridge();

	void dumpPaletteRam();
	void dumpVideoRam();

private:
	std::unique_ptr<IMemory> _palette_ram {nullptr};
	std::unique_ptr<IMemory> _video_ram {nullptr};
	Cartridge* _cartridge {nullptr};
};
