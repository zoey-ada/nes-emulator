#include "inesHeader.hpp"

InesHeader_1_0::InesHeader_1_0(const InesHeader& header)
	: _identifier(header.identifier),
	  _prg_rom_size(header.prg_rom_size),
	  _chr_rom_size(header.chr_rom_size),
	  _flags_6(header.flags_6),
	  _flags_7(header.flags_7)
{}

InesHeader_2_0::InesHeader_2_0(const InesHeader& header)
	: _identifier(header.identifier),
	  _prg_rom_size(header.prg_rom_size),
	  _chr_rom_size(header.chr_rom_size),
	  _flags_6(header.flags_6),
	  _flags_7(header.flags_7),
	  _flags_8(header.flags_8),
	  _flags_9(header.flags_9),
	  _flags_10(header.flags_10),
	  _flags_11(header.flags_11),
	  _flags_12(header.flags_12),
	  _flags_13(header.flags_13),
	  _flags_14(header.flags_14),
	  _flags_15(header.flags_15)
{}
