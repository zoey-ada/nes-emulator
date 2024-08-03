#include "mapper0.hpp"

#include <cassert>

Mapper0::Mapper0(bool has_second_bank): _has_second_bank(has_second_bank)
{}

uint16_t Mapper0::translate_address(uint16_t address) const
{
	if (address < 0x6000) [[unlikely]]
	{
		// shouldn't be here.
		assert(false);
	}
	else if (address >= 0xC000 && !this->_has_second_bank)
	{
		return address - 0xA000;
	}
	else [[likely]]
	{
		return address - 0x6000;
	}
}
