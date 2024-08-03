#include "cartridgeLoader.hpp"

#include <fstream>

#include "../iMemory.hpp"
#include "mappers/mapper0.hpp"

std::unique_ptr<Cartridge> CartridgeLoader::load_cartridge(const std::string& filepath)
{
	CartridgeData cart_data = this->load_rom_into_memory(filepath);
	InesVersion ines_version = this->get_version(cart_data);
	auto mapper = this->load_mapper(cart_data, ines_version);
	return std::make_unique<Cartridge>(cart_data.data, cart_data.data_size, std::move(mapper));
}

CartridgeData CartridgeLoader::load_rom_into_memory(const std::string& filepath)
{
	CartridgeData cart_data;

	std::ifstream rom_file(filepath.c_str(), std::ios::in | std::ios::binary);
	rom_file.seekg(0, std::ios::end);
	cart_data.data_size = rom_file.tellg();
	rom_file.seekg(0, std::ios::beg);

	cart_data.data = new (std::nothrow) uint8_t[cart_data.data_size];
	if (!cart_data.data)
	{
		throw std::exception("failed to create buffer for rom.");
	}

	rom_file.read((char*)cart_data.data, cart_data.data_size);

	return cart_data;
}

InesVersion CartridgeLoader::get_version(const CartridgeData& cart_data)
{
	InesHeader* header = (InesHeader*)cart_data.data;
	InesHeader_2_0 v2_header(*header);

	uint32_t total_rom_size = v2_header.prg_rom_size() + v2_header.chr_rom_size();
	if (v2_header.version() == 2 && total_rom_size <= cart_data.data_size)
		return InesVersion::ines_2_0;
	else if (v2_header.version() == 1)
		throw std::exception("cannot load archaic iNES ROM");
	else if (v2_header.version() == 0 && v2_header.is_v2_padding_zero())
		return InesVersion::ines_1_0;
	else
		throw std::exception("cannot load archaic iNES ROM");
}

std::unique_ptr<IMapper> CartridgeLoader::load_mapper(const CartridgeData& cart_data,
	InesVersion ines_version)
{
	if (ines_version == InesVersion::ines_2_0)
		throw std::exception("cannot load iNES v2 ROM");

	InesHeader* header = (InesHeader*)cart_data.data;
	InesHeader_1_0 v1_header(*header);

	if (v1_header.mapper_number() == 0)
	{
		bool has_two_mem_banks = (v1_header.prg_rom_size() == 2);
		return std::make_unique<Mapper0>(has_two_mem_banks);
	}

	return nullptr;
}
