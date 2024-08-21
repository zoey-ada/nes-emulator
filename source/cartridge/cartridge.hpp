#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "inesHeader_1_0.hpp"
#include "inesHeader_2_0.hpp"

class IMapper;
class IMemory;

struct CartridgeData_1_0
{
	InesHeader_1_0 header;
	std::vector<uint8_t> program_rom_data;
	std::vector<uint8_t> character_rom_data;
};

struct CartridgeData_2_0
{
	InesHeader_2_0 header;
	std::vector<uint8_t> program_rom_data;
	std::vector<uint8_t> character_rom_data;
	std::vector<uint8_t> misc_rom_data;
};

class Cartridge
{
public:
	Cartridge(CartridgeData_1_0 cart_data, std::unique_ptr<IMapper> mapper);
	Cartridge(CartridgeData_2_0 cart_data, std::unique_ptr<IMapper> mapper);
	virtual ~Cartridge();

	uint8_t read_program(uint16_t address) const;
	void write_program(uint16_t address, const uint8_t data);

	uint8_t read_character(uint16_t address) const;
	void write_character(uint16_t address, const uint8_t data);

	inline void setConsoleVideoRam(IMemory* video_ram) { this->_console_video_ram = video_ram; }

private:
	std::vector<uint8_t> _program_rom;
	std::vector<uint8_t> _character_rom;
	std::vector<uint8_t> _misc_rom;
	std::unique_ptr<IMapper> _mapper {nullptr};
	IMemory* _console_video_ram {nullptr};
};
