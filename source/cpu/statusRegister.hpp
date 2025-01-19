#pragma once

#include <base/register.hpp>

class StatusRegister: public Register_8bit
{
public:
	bool n_flag() const { return (this->_data & 0b10000000) > 0; }
	void n_flag(const bool state)
	{
		state ? this->_data |= 0b10000000 : this->_data &= 0b01111111;
	};

	bool v_flag() const { return (this->_data & 0b01000000) > 0; }
	void v_flag(const bool state)
	{
		state ? this->_data |= 0b01000000 : this->_data &= 0b10111111;
	};

	bool b_flag() const { return (this->_data & 0b00010000) > 0; }
	void b_flag(const bool state)
	{
		state ? this->_data |= 0b00010000 : this->_data &= 0b11101111;
	};

	bool d_flag() const { return (this->_data & 0b00001000) > 0; }
	void d_flag(const bool state)
	{
		state ? this->_data |= 0b00001000 : this->_data &= 0b11110111;
	};

	bool i_flag() const { return (this->_data & 0b00000100) > 0; }
	void i_flag(const bool state)
	{
		state ? this->_data |= 0b00000100 : this->_data &= 0b11111011;
	};

	bool z_flag() const { return (this->_data & 0b00000010) > 0; }
	void z_flag(const bool state)
	{
		state ? this->_data |= 0b00000010 : this->_data &= 0b11111101;
	};

	bool c_flag() const { return (this->_data & 0b00000001) > 0; }
	void c_flag(const bool state)
	{
		state ? this->_data |= 0b00000001 : this->_data &= 0b11111110;
	};
};
