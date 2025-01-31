#pragma once

#include <cstdint>

#include <base/register.hpp>

class Alu
{
public:
	Alu() = default;

	inline bool carry() const { return this->_carry; }
	inline bool overflow() const { return this->_overflow; }

	uint8_t add(uint8_t a, uint8_t b);
	uint8_t add(uint8_t a, uint8_t b, bool carry);

	uint8_t subtract(uint8_t a, uint8_t b);
	uint8_t subtract(uint8_t a, uint8_t b, bool borrow);

	uint8_t shift_left(uint8_t value);
	void shift_left(Register_8bit& value);
	uint8_t shift_right(uint8_t value);
	void shift_right(Register_8bit& value);

	uint8_t rotate_left(uint8_t value, bool carry);
	void rotate_left(Register_8bit& value, bool carry);
	uint8_t rotate_right(uint8_t value, bool carry);
	void rotate_right(Register_8bit& value, bool carry);

	inline bool get_bit(uint8_t value, uint8_t bit_num);

private:
	bool _carry = false;
	bool _overflow = false;
};

inline bool Alu::get_bit(uint8_t value, uint8_t bit_num)
{
	return (value & (1 << bit_num)) > 0;
}
