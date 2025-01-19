#pragma once

#include <array>
#include <cstdint>

class Register_16bit
{
public:
	uint16_t operator()() const { return this->_data; }
	void operator()(const uint16_t value) { this->_data = value; }

	uint8_t highByte() const { return static_cast<uint8_t>(this->_data >> 8); }
	uint8_t lowByte() const { return static_cast<uint8_t>(this->_data & 0xff); }

	void highByte(const uint8_t value) { this->_data = (this->_data & 0x00ff) | (value << 8); }
	void lowByte(const uint8_t value) { this->_data = (this->_data & 0xff00) | value; }

	// prefix increment
	Register_16bit& operator++()
	{
		this->_data++;
		return *this;
	}

	// postfix increment
	Register_16bit operator++(int)
	{
		Register_16bit old = *this;
		this->_data++;
		return old;
	}

	// prefix decrement
	Register_16bit& operator--()
	{
		this->_data--;
		return *this;
	}

	// postfix decrement
	Register_16bit operator--(int)
	{
		Register_16bit old = *this;
		this->_data--;
		return old;
	}

private:
	uint16_t _data {0x0000};
};

class Register_15bit
{
public:
	uint16_t operator()() const { return this->_data; }
	void operator()(const uint16_t value) { this->_data = (value & 0x7fff); }

	uint8_t highByte() const { return static_cast<uint8_t>(this->_data >> 8); }
	uint8_t lowByte() const { return static_cast<uint8_t>(this->_data & 0xff); }

	void highByte(const uint8_t value)
	{
		this->_data = (this->_data & 0x00ff) | ((value & 0x7f) << 8);
	}
	void lowByte(const uint8_t value) { this->_data = (this->_data & 0xff00) | value; }

	// prefix increment
	Register_15bit& operator++()
	{
		this->_data = (this->_data + 1) % 0x8000;
		return *this;
	}

	// postfix increment
	Register_15bit operator++(int)
	{
		Register_15bit old = *this;
		this->operator++();
		return old;
	}

	// prefix decrement
	Register_15bit& operator--()
	{
		if (this->_data == 0)
			this->_data = 0x7fff;
		else
			this->_data--;

		return *this;
	}

	// postfix decrement
	Register_15bit operator--(int)
	{
		Register_15bit old = *this;
		this->operator--();
		return old;
	}

private:
	uint16_t _data {0x0000};
};

class Register_14bit
{
public:
	uint16_t operator()() const { return this->_data; }
	void operator()(const uint16_t value) { this->_data = (value & 0x3fff); }

	uint8_t highByte() const { return static_cast<uint8_t>(this->_data >> 8); }
	uint8_t lowByte() const { return static_cast<uint8_t>(this->_data & 0xff); }

	void highByte(const uint8_t value)
	{
		this->_data = (this->_data & 0x00ff) | ((value & 0x3f) << 8);
	}
	void lowByte(const uint8_t value) { this->_data = (this->_data & 0xff00) | value; }

	// prefix increment
	Register_14bit& operator++()
	{
		this->_data = (this->_data + 1) % 0x4000;
		return *this;
	}

	// postfix increment
	Register_14bit operator++(int)
	{
		Register_14bit old = *this;
		this->operator++();
		return old;
	}

	// prefix decrement
	Register_14bit& operator--()
	{
		if (this->_data == 0)
			this->_data = 0x3fff;
		else
			this->_data--;

		return *this;
	}

	// postfix decrement
	Register_14bit operator--(int)
	{
		Register_14bit old = *this;
		this->operator--();
		return old;
	}

private:
	uint16_t _data {0x0000};
};

class Register_10bit
{
public:
	uint16_t operator()() const { return this->_data; }
	void operator()(const uint16_t value) { this->_data = (value & 0x03ff); }

	uint8_t highByte() const { return static_cast<uint8_t>(this->_data >> 8); }
	uint8_t lowByte() const { return static_cast<uint8_t>(this->_data & 0xff); }

	void highByte(const uint8_t value)
	{
		this->_data = (this->_data & 0x00ff) | ((value & 0x03) << 8);
	}
	void lowByte(const uint8_t value) { this->_data = (this->_data & 0xff00) | value; }

	// prefix increment
	Register_10bit& operator++()
	{
		this->_data = (this->_data + 1) % 0x0400;
		return *this;
	}

	// postfix increment
	Register_10bit operator++(int)
	{
		Register_10bit old = *this;
		this->operator++();
		return old;
	}

	// prefix decrement
	Register_10bit& operator--()
	{
		if (this->_data == 0)
			this->_data = 0x03ff;
		else
			this->_data--;

		return *this;
	}

	// postfix decrement
	Register_10bit operator--(int)
	{
		Register_10bit old = *this;
		this->operator--();
		return old;
	}

private:
	uint16_t _data {0x0000};
};

class Register_8bit
{
public:
	inline uint8_t operator()() const { return this->_data; }
	inline void operator()(const uint8_t value) { this->_data = value; }

	// prefix increment
	Register_8bit& operator++()
	{
		this->_data++;
		return *this;
	}

	// postfix increment
	Register_8bit operator++(int)
	{
		Register_8bit old = *this;
		this->_data++;
		return old;
	}

	// prefix decrement
	Register_8bit& operator--()
	{
		this->_data--;
		return *this;
	}

	// postfix decrement
	Register_8bit operator--(int)
	{
		Register_8bit old = *this;
		this->_data--;
		return old;
	}

protected:
	uint8_t _data {0x00};
};

class Register_6bit
{
public:
	inline uint8_t operator()() const { return this->_data; }
	inline void operator()(const uint8_t value) { this->_data = value & 0x3f; }

	// prefix increment
	Register_6bit& operator++()
	{
		this->_data = (this->_data + 1) % 0x40;
		return *this;
	}

	// postfix increment
	Register_6bit operator++(int)
	{
		Register_6bit old = *this;
		this->operator++();
		return old;
	}

	// prefix decrement
	Register_6bit& operator--()
	{
		if (this->_data == 0)
			this->_data = 0x3f;
		else
			this->_data--;

		return *this;
	}

	// postfix decrement
	Register_6bit operator--(int)
	{
		Register_6bit old = *this;
		this->operator--();
		return old;
	}

private:
	uint8_t _data {0x00};
};

class Register_5bit
{
public:
	inline uint8_t operator()() const { return this->_data; }
	inline void operator()(const uint8_t value) { this->_data = value & 0x1f; }

	// prefix increment
	Register_5bit& operator++()
	{
		this->_data = (this->_data + 1) % 0x20;
		return *this;
	}

	// postfix increment
	Register_5bit operator++(int)
	{
		Register_5bit old = *this;
		this->operator++();
		return old;
	}

	// prefix decrement
	Register_5bit& operator--()
	{
		if (this->_data == 0)
			this->_data = 0x1f;
		else
			this->_data--;

		return *this;
	}

	// postfix decrement
	Register_5bit operator--(int)
	{
		Register_5bit old = *this;
		this->operator--();
		return old;
	}

private:
	uint8_t _data {0x00};
};

class Register_3bit
{
public:
	inline uint8_t operator()() const { return this->_data; }
	inline void operator()(const uint8_t value) { this->_data = value & 0x07; }

	// prefix increment
	Register_3bit& operator++()
	{
		this->_data = (this->_data + 1) % 0x08;
		return *this;
	}

	// postfix increment
	Register_3bit operator++(int)
	{
		Register_3bit old = *this;
		this->operator++();
		return old;
	}

	// prefix decrement
	Register_3bit& operator--()
	{
		if (this->_data == 0)
			this->_data = 0x07;
		else
			this->_data--;

		return *this;
	}

	// postfix decrement
	Register_3bit operator--(int)
	{
		Register_3bit old = *this;
		this->operator--();
		return old;
	}

private:
	uint8_t _data {0x00};
};
