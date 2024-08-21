#pragma once

#include <memory>
#include <string>
#include <vector>

#include "cartridge.hpp"

class IMapper;

class CartridgeLoader
{
public:
	CartridgeLoader() = default;
	virtual ~CartridgeLoader() = default;

	std::unique_ptr<Cartridge> load_cartridge(const std::string& filepath);

private:
	std::unique_ptr<IMapper> load_v1_mapper(const CartridgeData_1_0& cart_data);
	std::unique_ptr<IMapper> load_v2_mapper(const CartridgeData_2_0& cart_data);
};
