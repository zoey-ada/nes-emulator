#pragma once

#include <cstdint>

class ICpu
{
public:
	virtual ~ICpu() = default;

	virtual void cycle() = 0;
	virtual void cycle(const uint64_t number_of_cycles) = 0;

	virtual uint16_t address_bus() const = 0;
	virtual uint8_t data_bus() const = 0;
	virtual void data_bus(const uint8_t data) = 0;

	virtual bool audio_out_1() const = 0;
	virtual bool audio_out_2() const = 0;
	virtual bool read_write() const = 0;

	virtual void irq() = 0;
	virtual void nmi() = 0;
	virtual void reset() = 0;
};
