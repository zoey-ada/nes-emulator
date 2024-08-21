#include "cartridgeLoader.hpp"

#include <array>
#include <fstream>

#include <base/iMemory.hpp>

#include "inesHeader.hpp"
#include "inesHeader_1_0.hpp"
#include "inesHeader_2_0.hpp"
#include "mappers/mapper0.hpp"

std::unique_ptr<Cartridge> CartridgeLoader::load_cartridge(const std::string& filepath)
{
	std::ifstream rom_file(filepath.c_str(), std::ios::in | std::ios::binary);
	rom_file.seekg(0, std::ios::end);
	uint64_t rom_size = rom_file.tellg();

	InesHeader header_data;
	rom_file.seekg(0, std::ios::beg);
	rom_file.read((char*)&header_data, 16);
	auto ines_version = get_header_version(header_data, rom_size);

	std::vector<uint8_t> program_rom;
	std::vector<uint8_t> character_rom;
	std::vector<uint8_t> misc_rom;

	if (ines_version == InesVersion::ines_1_0)
	{
		InesHeader_1_0 header(header_data);
		program_rom.resize(header.program_rom_size());
		character_rom.resize(header.character_rom_size());

		rom_file.seekg(header.trainer_data_size(), std::ios::cur);  // skip over the trainer data
		rom_file.read((char*)program_rom.data(), program_rom.size());
		rom_file.read((char*)character_rom.data(), character_rom.size());

		CartridgeData_1_0 cart_data {header, program_rom, character_rom};
		auto mapper = this->load_v1_mapper(cart_data);
		return std::make_unique<Cartridge>(std::move(cart_data), std::move(mapper));
	}
	else if (ines_version == InesVersion::ines_2_0)
	{
		InesHeader_2_0 header(header_data);
		uint64_t misc_rom_size = rom_size - 16 - header.trainer_data_size() -
			header.program_rom_size() - header.character_rom_size();

		program_rom.resize(header.program_rom_size());
		character_rom.resize(header.character_rom_size());
		misc_rom.resize(misc_rom_size);

		rom_file.seekg(header.trainer_data_size(), std::ios::cur);  // skip over the trainer data
		rom_file.read((char*)program_rom.data(), program_rom.size());
		rom_file.read((char*)character_rom.data(), character_rom.size());
		rom_file.read((char*)misc_rom.data(), misc_rom.size());

		CartridgeData_2_0 cart_data {header, program_rom, character_rom};
		auto mapper = this->load_v2_mapper(cart_data);
		return std::make_unique<Cartridge>(std::move(cart_data), std::move(mapper));
	}
	else
	{
		throw std::exception("could not load ROM: invalid header type");
	}
}

std::unique_ptr<IMapper> CartridgeLoader::load_v1_mapper(const CartridgeData_1_0& cart_data)
{
	if (cart_data.header.mapper_number() == 0)
	{
		bool has_two_mem_banks = (cart_data.header.program_rom_size() == 32768);
		bool has_vertical_nametable_mirror =
			cart_data.header.nametable_arrangement() == NameTableArrangement::vertical;
		return std::make_unique<Mapper0>(has_two_mem_banks, has_vertical_nametable_mirror);
	}

	return nullptr;
}

std::unique_ptr<IMapper> CartridgeLoader::load_v2_mapper(const CartridgeData_2_0& cart_data)
{
	return nullptr;
}
