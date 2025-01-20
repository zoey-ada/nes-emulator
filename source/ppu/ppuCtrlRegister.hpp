#pragma once

#include <base/register.hpp>

class PpuCtrlRegister: public Register_8bit
{
public:
	bool nmi_enable_flag() const { return (this->_data & 0b1000'0000) > 0; }
	void nmi_enable_flag(const bool enabled)
	{
		enabled ? this->_data |= 0b1000'0000 : this->_data &= 0b0111'1111;
	}

	bool ppu_master_slave_flag() const { return (this->_data & 0b0100'0000) > 0; }
	void ppu_master_slave_flag(const bool is_slave)
	{
		is_slave ? this->_data |= 0b0100'0000 : this->_data &= 0b1011'1111;
	}

	bool sprite_height_flag() const { return (this->_data & 0b0010'0000) > 0; }
	void sprite_height_flag(const bool is_8x16)
	{
		is_8x16 ? this->_data |= 0b0010'0000 : this->_data &= 0b1101'1111;
	}

	bool background_pattern_table_select_flag() const { return (this->_data & 0b0001'0000) > 0; }
	void background_pattern_table_select_flag(const bool select_right)
	{
		select_right ? this->_data |= 0b0001'0000 : this->_data &= 0b1110'1111;
	}

	bool sprite_pattern_table_select_flag() const { return (this->_data & 0b0000'1000) > 0; }
	void sprite_pattern_table_select_flag(const bool select_right)
	{
		select_right ? this->_data |= 0b0000'1000 : this->_data &= 0b1111'0111;
	}

	bool increment_mode_flag() const { return (this->_data & 0b0000'0100) > 0; }
	void increment_mode_flag(const bool should_increment_32)
	{
		should_increment_32 ? this->_data |= 0b0000'0100 : this->_data &= 0b1111'1011;
	}

	uint8_t nametable_select() const { return this->_data & 0b0000'0011; }
	void nametable_select(const uint8_t select)
	{
		this->_data &= 0b1111'1100;
		this->_data |= (select & 0b0000'0011);
	}
};
