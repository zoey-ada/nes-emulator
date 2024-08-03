#pragma once

#include <cstdint>
#include <memory>

class IMapper;

class Cartridge
{
public:
	Cartridge(uint8_t* rom, uint32_t rom_size, std::unique_ptr<IMapper> mapper);
	virtual ~Cartridge();

	uint8_t read(uint16_t address) const;
	void write(uint16_t address, const uint8_t data);

private:
	uint8_t* _rom {nullptr};
	uint32_t _rom_size {0};
	std::unique_ptr<IMapper> _mapper {nullptr};
};
