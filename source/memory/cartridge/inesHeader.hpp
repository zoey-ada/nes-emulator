#pragma once

#include <cstdint>

enum class NameTableArrangement
{
	vertical,
	horizontal,
	alternative
};

enum class InesVersion
{
	ines_1_0,
	ines_2_0,
	unsupported
};

enum class ConsoleType
{
	nintendo_entertainment_system,
	nintento_vs_system,
	nintendo_playchoice_10,
	famiclone_with_decimal_mode_cpu,
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

InesVersion get_header_version(InesHeader header_data, uint64_t rom_size);
