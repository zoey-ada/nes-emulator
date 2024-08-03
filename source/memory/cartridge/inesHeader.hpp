#pragma once

#include <cstdint>

enum class NameTableArrangement
{
	vertical,
	horizontal
};

enum class InesVersion
{
	ines_1_0,
	ines_2_0
};

enum class ConsoleType
{
	nintendo_entertainment_system,
	nintento_vs_system,
	nintendo_playchoice_10,
	extended_console_type
};

enum class TimingMode
{
	ntsc,
	pal,
	multi_region,
	dendy
};

enum class VsSystemPpuType
{
	RP2C03B,
	RP2C03G,
	RP2C04_0001,
	RP2C04_0002,
	RP2C04_0003,
	RP2C04_0004,
	RC2C03B,
	RC2C03C,
	RC2C05_01,
	RC2C05_02,
	RC2C05_03,
	RC2C05_04,
	RC2C05_05
};

enum class VsSystemHardwareType
{
	unisystem_normal,
	unisystem_rbi_baseball,
	unisystem_tko_boxing,
	unisystem_super_xevious,
	unisystem_vs_ice_climber_japan,
	dual_system_normal,
	dual_system_raid_on_bungeling_bay
};

enum class ExtendedConsoleType
{
	famiclone_with_decimal_mode_cpu = 3,
	famicom_with_epsm_module_or_plug_through_cartridge,
	vr_technology_vt01,
	vr_technology_vt02,
	vr_technology_vt03,
	vr_technology_vt09,
	vr_technology_vt32,
	vr_technology_vt369,
	umc_um6578,
	famicom_network_system
};

struct InesHeader
{
	uint32_t identifier;
	uint8_t prg_rom_size;
	uint8_t chr_rom_size;
	uint8_t flags_6;
	uint8_t flags_7;
	uint8_t flags_8;
	uint8_t flags_9;
	uint8_t flags_10;
	uint8_t flags_11;
	uint8_t flags_12;
	uint8_t flags_13;
	uint8_t flags_14;
	uint8_t flags_15;
};

class InesHeader_1_0
{
public:
	InesHeader_1_0(const InesHeader& header);

	inline uint8_t prg_rom_size() const { return _prg_rom_size; }
	inline uint8_t chr_rom_size() const { return _chr_rom_size; }
	inline NameTableArrangement name_table_arrangement() const
	{
		return this->_flags_6 & 0b00000001 ? NameTableArrangement::horizontal :
											 NameTableArrangement::vertical;
	}
	inline bool has_battery_backed_prg_ram() const { return (this->_flags_6 & 0b00000010) > 0; }
	inline bool has_512_byte_trainer() const { return (this->_flags_6 & 0b00000100) > 0; }
	inline bool has_alt_nametable_layout() const { return (this->_flags_6 & 0b00001000) > 0; }
	inline uint8_t mapper_number() const
	{
		return this->_flags_7 & 0b11110000 | ((this->_flags_6 & 0b11110000) >> 4);
	}
	inline uint8_t version() const { return (this->_flags_7 & 0b00001100) >> 2; }

private:
	uint32_t _identifier {0};
	uint8_t _prg_rom_size {0};
	uint8_t _chr_rom_size {0};
	uint8_t _flags_6 {0};
	uint8_t _flags_7 {0};
	uint64_t _padding {0};
};

class InesHeader_2_0
{
public:
	InesHeader_2_0(const InesHeader& header);

	inline uint16_t prg_rom_size() const
	{
		return (static_cast<uint16_t>(this->_flags_9 & 0b00001111) << 8) |
			static_cast<uint16_t>(this->_prg_rom_size);
	}
	inline uint16_t chr_rom_size() const
	{
		return (static_cast<uint16_t>(this->_flags_9 & 0b11110000) << 4) |
			static_cast<uint16_t>(this->_chr_rom_size);
	}
	inline NameTableArrangement name_table_arrangement() const
	{
		return this->_flags_6 & 0b00000001 ? NameTableArrangement::horizontal :
											 NameTableArrangement::vertical;
	}
	inline bool has_battery_backed_prg_ram() const { return (this->_flags_6 & 0b00000010) > 0; }
	inline bool has_512_byte_trainer() const { return (this->_flags_6 & 0b00000100) > 0; }
	inline bool has_alt_nametable_layout() const { return (this->_flags_6 & 0b00001000) > 0; }
	inline uint8_t mapper_number() const
	{
		return (static_cast<uint16_t>(this->_flags_8 & 0b00001111) << 8) |
			static_cast<uint16_t>(this->_flags_7 & 0b11110000) |
			(static_cast<uint16_t>(this->_flags_6 & 0b11110000) >> 4);
	}
	inline ConsoleType console_type() const
	{
		return static_cast<ConsoleType>(this->_flags_7 & 0b00000011);
	}
	inline uint8_t version() const { return (this->_flags_7 & 0b00001100) >> 2; }
	inline uint8_t submapper_number() const { return (this->_flags_8 & 0b11110000) >> 4; }
	inline uint32_t prg_ram_size() const
	{
		return (this->_flags_10 & 0b11110000) > 0 ?
			static_cast<uint32_t>(64) << (this->_flags_10 & 0b00001111) :
			0;
	}
	inline uint32_t prg_nvram_size() const
	{
		return (this->_flags_10 & 0b11110000) > 0 ?
			static_cast<uint32_t>(64) << ((this->_flags_10 & 0b11110000) >> 4) :
			0;
	}
	inline uint32_t chr_ram_size() const
	{
		return (this->_flags_11 & 0b11110000) > 0 ?
			static_cast<uint32_t>(64) << (this->_flags_11 & 0b00001111) :
			0;
	}
	inline uint32_t chr_nvram_size() const
	{
		return (this->_flags_11 & 0b11110000) > 0 ?
			static_cast<uint32_t>(64) << ((this->_flags_11 & 0b11110000) >> 4) :
			0;
	}
	inline TimingMode timing_mode() const
	{
		return static_cast<TimingMode>(this->_flags_12 & 0b00000011);
	}
	inline VsSystemHardwareType vs_system_hardware_type() const
	{
		return static_cast<VsSystemHardwareType>((this->_flags_13 & 0b11110000) >> 4);
	}
	inline VsSystemPpuType vs_system_ppu_type() const
	{
		return static_cast<VsSystemPpuType>(this->_flags_13 & 0b00001111);
	}
	inline VsSystemPpuType extended_console_type() const
	{
		return static_cast<VsSystemPpuType>(this->_flags_13 & 0b00001111);
	}
	inline uint8_t misc_rom_count() const { return this->_flags_14 & 0b00000011; }
	inline uint8_t default_expansion_device() const { return this->_flags_15 & 0b00111111; }

	inline bool is_v2_padding_zero()
	{
		return (this->_flags_12 | this->_flags_13 | this->_flags_14 | this->_flags_15) == 0;
	}

private:
	uint32_t _identifier;
	uint8_t _prg_rom_size;
	uint8_t _chr_rom_size;
	uint8_t _flags_6;
	uint8_t _flags_7;
	uint8_t _flags_8;
	uint8_t _flags_9;
	uint8_t _flags_10;
	uint8_t _flags_11;
	uint8_t _flags_12;
	uint8_t _flags_13;
	uint8_t _flags_14;
	uint8_t _flags_15;
};
