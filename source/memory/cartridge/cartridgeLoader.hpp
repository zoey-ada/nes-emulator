#pragma once

#include <memory>
#include <string>

#include "cartridge.hpp"
#include "inesHeader.hpp"

class IMapper;

struct CartridgeData
{
	uint8_t* data {nullptr};
	uint32_t data_size {0};
};

class CartridgeLoader
{
public:
	CartridgeLoader() = default;
	virtual ~CartridgeLoader() = default;

	std::unique_ptr<Cartridge> load_cartridge(const std::string& filepath);

private:
	CartridgeData load_rom_into_memory(const std::string& filepath);
	InesVersion get_version(const CartridgeData& cart_data);
	std::unique_ptr<IMapper> load_mapper(const CartridgeData& cart_data, InesVersion ines_version);
};
