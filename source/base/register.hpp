#pragma once

#include <array>
#include <cstdint>

class Register_16bit
{
public:
	inline uint16_t operator()() const { return (uint16_t&)this->_data[0]; }
	inline void operator()(uint16_t value) { (uint16_t&)this->_data[0] = std::move(value); }

	inline uint8_t highByte() const { return this->_data[1]; }
	inline uint8_t lowByte() const { return this->_data[0]; }

	inline void highByte(uint8_t value) { this->_data[1] = std::move(value); }
	inline void lowByte(uint8_t value) { this->_data[0] = std::move(value); }

	inline void operator++() { ((uint16_t&)this->_data[0])++; }
	inline void operator--() { ((uint16_t&)this->_data[0])--; }

private:
	std::array<uint8_t, 2> _data {0x00, 0x00};
};

class Register_15bit
{
public:
	inline uint16_t operator()() const { return (uint16_t&)this->_data[0]; }
	inline void operator()(const uint16_t value) { (uint16_t&)this->_data[0] = value & 0x7fff; }

	inline uint8_t highByte() const { return this->_data[1]; }
	inline uint8_t lowByte() const { return this->_data[0]; }

	inline void highByte(const uint8_t value) { this->_data[1] = value & 0x7f; }
	inline void lowByte(uint8_t value) { this->_data[0] = std::move(value); }

	inline void operator++() { (uint16_t&)this->_data[0] = ((*this)() + 1) % 0x8000; }
	inline void operator--()
	{
		if ((*this)() == 0)
			(uint16_t&)this->_data[0] = 0x7fff;
		else
			((uint16_t&)this->_data[0])--;
	}

private:
	std::array<uint8_t, 2> _data {0x00, 0x00};
};

class Register_14bit
{
public:
	inline uint16_t operator()() const { return (uint16_t&)this->_data[0]; }
	inline void operator()(const uint16_t value) { (uint16_t&)this->_data[0] = value & 0x3fff; }

	inline uint8_t highByte() const { return this->_data[1]; }
	inline uint8_t lowByte() const { return this->_data[0]; }

	inline void highByte(const uint8_t value) { this->_data[1] = value & 0x3f; }
	inline void lowByte(uint8_t value) { this->_data[0] = std::move(value); }

	inline void operator++() { (uint16_t&)this->_data[0] = ((*this)() + 1) % 0x4000; }
	inline void operator--()
	{
		if ((*this)() == 0)
			(uint16_t&)this->_data[0] = 0x3fff;
		else
			((uint16_t&)this->_data[0])--;
	}

private:
	std::array<uint8_t, 2> _data {0x00, 0x00};
};

class Register_10bit
{
public:
	inline uint16_t operator()() const { return (uint16_t&)this->_data[0]; }
	inline void operator()(const uint16_t value) { (uint16_t&)this->_data[0] = value & 0x03ff; }

	inline uint8_t highByte() const { return this->_data[1]; }
	inline uint8_t lowByte() const { return this->_data[0]; }

	inline void highByte(const uint8_t value) { this->_data[1] = value & 0x03; }
	inline void lowByte(uint8_t value) { this->_data[0] = std::move(value); }

	inline void operator++() { (uint16_t&)this->_data[0] = ((*this)() + 1) % 0x0400; }
	inline void operator--()
	{
		if ((*this)() == 0)
			(uint16_t&)this->_data[0] = 0x03ff;
		else
			((uint16_t&)this->_data[0])--;
	}

private:
	std::array<uint8_t, 2> _data {0x00, 0x00};
};

class Register_8bit
{
public:
	inline uint8_t operator()() const { return this->_data; }
	inline void operator()(uint8_t value) { this->_data = std::move(value); }

	inline void operator++() { this->_data++; }
	inline void operator--() { this->_data--; }

private:
	uint8_t _data {0x00};
};

class Register_6bit
{
public:
	inline uint8_t operator()() const { return this->_data; }
	inline void operator()(const uint8_t value) { this->_data = value & 0x3f; }

	inline void operator++() { this->_data = (this->_data + 1) % 0x40; }
	inline void operator--()
	{
		if (this->_data == 0)
			this->_data = 0x3f;
		else
			this->_data--;
	}

private:
	uint8_t _data {0x00};
};

class Register_5bit
{
public:
	inline uint8_t operator()() const { return this->_data; }
	inline void operator()(const uint8_t value) { this->_data = value & 0x1f; }

	inline void operator++() { this->_data = (this->_data + 1) % 0x20; }
	inline void operator--()
	{
		if (this->_data == 0)
			this->_data = 0x1f;
		else
			this->_data--;
	}

private:
	uint8_t _data {0x00};
};

class Register_3bit
{
public:
	inline uint8_t operator()() const { return this->_data; }
	inline void operator()(const uint8_t value) { this->_data = value & 0x07; }

	inline void operator++() { this->_data = (this->_data + 1) % 0x08; }
	inline void operator--()
	{
		if (this->_data == 0)
			this->_data = 0x07;
		else
			this->_data--;
	}

private:
	uint8_t _data {0x00};
};
