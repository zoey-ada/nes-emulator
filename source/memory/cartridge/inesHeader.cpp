#include "inesHeader.hpp"

#include "inesHeader_2_0.hpp"

InesVersion get_header_version(InesHeader header_data, uint64_t rom_size)
{
	uint8_t version = (header_data.flags_7 & 0b00001100) >> 2;

	if (version == 0)
	{
		bool v2_padding_is_empty = header_data.flags_12 == 0 && header_data.flags_13 == 0 &&
			header_data.flags_14 == 0 && header_data.flags_15 == 0;

		if (v2_padding_is_empty)
			return InesVersion::ines_1_0;
	}
	else if (version == 2)
	{
		InesHeader_2_0 v2_header(header_data);
		uint32_t total_rom_size = v2_header.program_rom_size() + v2_header.character_rom_size();

		if (total_rom_size <= rom_size)
			return InesVersion::ines_2_0;
	}

	return InesVersion::unsupported;
}
