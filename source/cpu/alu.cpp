#include "alu.hpp"

uint8_t Alu::add(uint8_t a, uint8_t b)
{
	uint16_t sum = static_cast<uint16_t>(a) + static_cast<uint16_t>(b);
	this->_carry = (sum & 0x0100) > 0;
	uint8_t actual_sum = static_cast<uint8_t>(sum & 0x00ff);

	// this->_overflow = this->_carry != carry_6;
	bool terms_same_sign = (((a ^ b) & 0x80) == 0);
	bool sum_sign_match_a = (((a ^ actual_sum) & 0x80) == 0);
	this->_overflow = terms_same_sign && !sum_sign_match_a;

	return actual_sum;
}

uint8_t Alu::add(uint8_t a, uint8_t b, bool carry)
{
	uint16_t sum = static_cast<uint16_t>(a) + static_cast<uint16_t>(b) + (carry ? 1 : 0);
	this->_carry = (sum & 0x0100) > 0;
	uint8_t actual_sum = static_cast<uint8_t>(sum & 0x00ff);

	bool terms_same_sign = (((a ^ b) & 0x80) == 0);
	bool sum_sign_match_a = (((a ^ actual_sum) & 0x80) == 0);
	this->_overflow = terms_same_sign && !sum_sign_match_a;

	return actual_sum;
}

uint8_t Alu::subtract(uint8_t a, uint8_t b)
{
	uint8_t twos_comp = ~b + 1;
	uint8_t difference = this->add(a, twos_comp);

	// bool terms_same_sign = (((a ^ twos_comp) & 0x80) == 0);
	// bool sum_sign_match_a = (((a ^ difference) & 0x80) == 0);
	// this->_overflow = terms_same_sign && !sum_sign_match_a;

	return difference;
}

// A = A - b - (1 - C) -> A = A + (-b) + (c - 1)
uint8_t Alu::subtract(uint8_t a, uint8_t b, bool borrow)
{
	uint8_t twos_comp = ~b + 1;
	uint8_t difference = this->add(a, twos_comp);

	if (borrow)
	{
		auto carry = this->_carry;
		difference = this->add(difference, 0xff);
		this->_carry = this->_carry && carry;
	}

	if (((a ^ b) == 0) && !borrow)
		this->_carry = true;

	// bool terms_same_sign = (((a ^ twos_comp) & 0x80) == 0);
	// bool sum_sign_match_a = (((a ^ difference) & 0x80) == 0);
	// this->_overflow = terms_same_sign && !sum_sign_match_a;

	return difference;
}

uint8_t Alu::increment(uint8_t value)
{
	return ++value;
}

uint8_t Alu::decrement(uint8_t value)
{
	return --value;
}

uint8_t Alu::shift_left(uint8_t value)
{
	this->_carry = (value & 0b10000000) > 0;
	return value << 1;
}

uint8_t Alu::shift_right(uint8_t value)
{
	this->_carry = (value & 0b00000001) > 0;
	return value >> 1;
}

uint8_t Alu::rotate_left(uint8_t value, bool carry)
{
	this->_carry = (value & 0b10000000) > 0;
	value = value << 1;
	return value | (carry ? 0b0000001 : 0b0000000);
}

uint8_t Alu::rotate_right(uint8_t value, bool carry)
{
	this->_carry = (value & 0b00000001) > 0;
	value = value >> 1;
	return value | (carry ? 0b10000000 : 0b0000000);
}
