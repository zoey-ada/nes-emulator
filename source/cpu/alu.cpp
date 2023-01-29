#include "alu.hpp"

uint8_t Alu::add(uint8_t a, uint8_t b)
{
	// uint8_t carry_6_sum = (a & 0x7f) + (b & 0x7f);
	// bool carry_6 = (carry_6_sum & 0x80) > 0;

	uint16_t sum = static_cast<uint16_t>(a) + static_cast<uint16_t>(b);
	this->_carry = (sum & 0x0100) > 0;

	// this->_overflow = this->_carry != carry_6;
	this->_overflow = (((a ^ b) & 0x80) == 0) && (((a ^ sum) & 0x80) > 0);
	return static_cast<uint8_t>(sum & 0x00ff);
}

uint8_t Alu::add(uint8_t a, uint8_t b, bool carry)
{
	// uint8_t carry_6_sum = (a & 0x7f) + (b & 0x7f);
	// bool carry_6 = (carry_6_sum & 0x80) > 0;

	uint16_t sum = static_cast<uint16_t>(a) + static_cast<uint16_t>(b) + (carry ? 1 : 0);
	this->_carry = (sum & 0x0100) > 0;

	// this->_overflow = this->_carry  != carry_6;
	this->_overflow = (((a ^ b) & 0x80) == 0) && (((a ^ sum) & 0x80) > 0);
	return static_cast<uint8_t>(sum & 0x00ff);
}

uint8_t Alu::subtract(uint8_t a, uint8_t b)
{
	uint8_t difference = this->add(a, ~b);
	this->_overflow = (((a ^ b) & 0x80) > 0) && (((a ^ difference) & 0x80) > 0);
	return difference;
}

uint8_t Alu::subtract(uint8_t a, uint8_t b, bool borrow)
{
	uint8_t difference = this->add(a, ~b, !borrow);
	this->_overflow = (((a ^ b) & 0x80) > 0) && (((a ^ difference) & 0x80) > 0);
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
