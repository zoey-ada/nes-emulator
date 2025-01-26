#include <cartridge/mappers/mapper0.hpp>
#include <catch2/catch_test_macros.hpp>

SCENARIO("Can translate program addresses", "[cartridge][mapper0]")
{
	// CPU $6000-$7FFF: Family Basic only: PRG RAM, mirrored as necessary to fill entire 8 KiB
	// 	 window, write protectable with an external switch
	// CPU $8000-$BFFF: First 16 KB of ROM.
	// CPU $C000-$FFFF: Last 16 KB of ROM (NROM-256) or mirror of $8000-$BFFF (NROM-128).
	GIVEN("cartridge has single memory bank")
	{
		Mapper0Options options;
		options.has_two_banks = false;
		options.has_vertical_nametable_mirroring = false;
		options.uses_character_ram = false;

		Mapper0 mapper(options);

		WHEN("address is 0x0000")
		{
			uint16_t address = 0x0000;

			THEN("exception is thrown")
			{
				REQUIRE_THROWS(mapper.translate_program_address(address));
			}
		}

		WHEN("address is 0x5fff")
		{
			uint16_t address = 0x5fff;

			THEN("exception is thrown")
			{
				REQUIRE_THROWS(mapper.translate_program_address(address));
			}
		}

		WHEN("address is 0x6000")
		{
			uint16_t address = 0x6000;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x7fff")
		{
			uint16_t address = 0x7fff;

			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRam);
				REQUIRE(translated_address.address == 0x1fff);
			}
		}

		WHEN("address is 0x8000")
		{
			uint16_t address = 0x8000;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to first bank of cartridge ROM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRom);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0xbfff")
		{
			uint16_t address = 0xbfff;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to first bank of cartridge ROM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRom);
				REQUIRE(translated_address.address == 0x3fff);
			}
		}

		WHEN("address is 0xc000")
		{
			uint16_t address = 0xc000;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to first bank of cartridge ROM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRom);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0xffff")
		{
			uint16_t address = 0xffff;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to first bank of cartridge ROM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRom);
				REQUIRE(translated_address.address == 0x3fff);
			}
		}
	}

	GIVEN("cartridge has two memory bank")
	{
		Mapper0Options options;
		options.has_two_banks = true;
		options.has_vertical_nametable_mirroring = false;
		options.uses_character_ram = false;

		Mapper0 mapper(options);

		WHEN("address is 0x0000")
		{
			uint16_t address = 0x0000;

			THEN("exception is thrown")
			{
				REQUIRE_THROWS(mapper.translate_program_address(address));
			}
		}

		WHEN("address is 0x5fff")
		{
			uint16_t address = 0x5fff;

			THEN("exception is thrown")
			{
				REQUIRE_THROWS(mapper.translate_program_address(address));
			}
		}

		WHEN("address is 0x6000")
		{
			uint16_t address = 0x6000;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x7fff")
		{
			uint16_t address = 0x7fff;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRam);
				REQUIRE(translated_address.address == 0x1fff);
			}
		}

		WHEN("address is 0x8000")
		{
			uint16_t address = 0x8000;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to first bank of cartridge ROM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRom);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0xbfff")
		{
			uint16_t address = 0xbfff;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to first bank of cartridge ROM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRom);
				REQUIRE(translated_address.address == 0x3fff);
			}
		}

		WHEN("address is 0xc000")
		{
			uint16_t address = 0xc000;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to second bank of cartridge ROM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRom);
				REQUIRE(translated_address.address == 0x4000);
			}
		}

		WHEN("address is 0xffff")
		{
			uint16_t address = 0xffff;
			auto translated_address = mapper.translate_program_address(address);

			THEN("address is mapped to second bank of cartridge ROM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRom);
				REQUIRE(translated_address.address == 0x7fff);
			}
		}
	}
}

SCENARIO("Can translate character addresses", "[cartridge][mapper0]")
{
	GIVEN("cartridge has character ram")
	{
		Mapper0Options options;
		options.has_two_banks = false;
		options.has_vertical_nametable_mirroring = false;
		options.uses_character_ram = true;

		Mapper0 mapper(options);

		WHEN("address is 0x0000")
		{
			uint16_t address = 0x0000;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x1fff")
		{
			uint16_t address = 0x1fff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRam);
				REQUIRE(translated_address.address == 0x1fff);
			}
		}

		WHEN("address is 0x4000")
		{
			uint16_t address = 0x4000;

			THEN("exception is thrown")
			{
				REQUIRE_THROWS(mapper.translate_character_address(address));
			}
		}
	}

	GIVEN("cartridge doesn't have character ram")
	{
		Mapper0Options options;
		options.has_two_banks = false;
		options.has_vertical_nametable_mirroring = false;
		options.uses_character_ram = false;

		Mapper0 mapper(options);

		WHEN("address is 0x0000")
		{
			uint16_t address = 0x0000;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRom);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x1fff")
		{
			uint16_t address = 0x1fff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::CartridgeRom);
				REQUIRE(translated_address.address == 0x1fff);
			}
		}

		WHEN("address is 0x4000")
		{
			uint16_t address = 0x4000;

			THEN("exception is thrown")
			{
				REQUIRE_THROWS(mapper.translate_character_address(address));
			}
		}
	}

	GIVEN("cartridge has pattern table vertical mirroring")
	{
		Mapper0Options options;
		options.has_two_banks = false;
		options.has_vertical_nametable_mirroring = true;
		options.uses_character_ram = false;

		Mapper0 mapper(options);

		WHEN("address is 0x2000")
		{
			uint16_t address = 0x2000;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x23ff")
		{
			uint16_t address = 0x23ff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x03ff);
			}
		}

		WHEN("address is 0x2400")
		{
			uint16_t address = 0x2400;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0400);
			}
		}

		WHEN("address is 0x27ff")
		{
			uint16_t address = 0x27ff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x07ff);
			}
		}

		WHEN("address is 0x2800")
		{
			uint16_t address = 0x2800;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x2bff")
		{
			uint16_t address = 0x2bff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x03ff);
			}
		}

		WHEN("address is 0x2c00")
		{
			uint16_t address = 0x2c00;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0400);
			}
		}

		WHEN("address is 0x2fff")
		{
			uint16_t address = 0x2fff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x07ff);
			}
		}

		WHEN("address is 0x3000")
		{
			uint16_t address = 0x3000;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x33ff")
		{
			uint16_t address = 0x33ff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x03ff);
			}
		}

		WHEN("address is 0x3400")
		{
			uint16_t address = 0x3400;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0400);
			}
		}

		WHEN("address is 0x37ff")
		{
			uint16_t address = 0x37ff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x07ff);
			}
		}

		WHEN("address is 0x3800")
		{
			uint16_t address = 0x3800;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x3bff")
		{
			uint16_t address = 0x3bff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x03ff);
			}
		}

		WHEN("address is 0x3c00")
		{
			uint16_t address = 0x3c00;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0400);
			}
		}

		WHEN("address is 0x3eff")
		{
			uint16_t address = 0x3eff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x06ff);
			}
		}
	}

	GIVEN("cartridge has pattern table horizontal mirroring")
	{
		Mapper0Options options;
		options.has_two_banks = false;
		options.has_vertical_nametable_mirroring = false;
		options.uses_character_ram = false;

		Mapper0 mapper(options);

		WHEN("address is 0x2000")
		{
			uint16_t address = 0x2000;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x23ff")
		{
			uint16_t address = 0x23ff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x03ff);
			}
		}

		WHEN("address is 0x2400")
		{
			uint16_t address = 0x2400;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x27ff")
		{
			uint16_t address = 0x27ff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x03ff);
			}
		}

		WHEN("address is 0x2800")
		{
			uint16_t address = 0x2800;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0400);
			}
		}

		WHEN("address is 0x2bff")
		{
			uint16_t address = 0x2bff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x07ff);
			}
		}

		WHEN("address is 0x2c00")
		{
			uint16_t address = 0x2c00;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0400);
			}
		}

		WHEN("address is 0x2fff")
		{
			uint16_t address = 0x2fff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x07ff);
			}
		}

		WHEN("address is 0x3000")
		{
			uint16_t address = 0x3000;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x33ff")
		{
			uint16_t address = 0x33ff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x03ff);
			}
		}

		WHEN("address is 0x3400")
		{
			uint16_t address = 0x3400;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0000);
			}
		}

		WHEN("address is 0x37ff")
		{
			uint16_t address = 0x37ff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x03ff);
			}
		}

		WHEN("address is 0x3800")
		{
			uint16_t address = 0x3800;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0400);
			}
		}

		WHEN("address is 0x3bff")
		{
			uint16_t address = 0x3bff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x07ff);
			}
		}

		WHEN("address is 0x3c00")
		{
			uint16_t address = 0x3c00;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x0400);
			}
		}

		WHEN("address is 0x3eff")
		{
			uint16_t address = 0x3eff;
			auto translated_address = mapper.translate_character_address(address);

			THEN("address is mapped to cartridge RAM")
			{
				REQUIRE(translated_address.device == MemoryDevice::ConsoleRam);
				REQUIRE(translated_address.address == 0x06ff);
			}
		}
	}
}
