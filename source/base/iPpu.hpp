#pragma once

#include <cstdint>

class ICpu;

class IPpu
{
public:
	virtual ~IPpu() = default;

	virtual void init(ICpu* cpu) = 0;
	virtual void reset() = 0;

	virtual void cycle() = 0;
	virtual void cycle(const uint64_t number_of_cycles) = 0;

	virtual uint16_t address_bus() const = 0;
	virtual void address_bus(const uint16_t data) = 0;
	virtual uint8_t data_bus() const = 0;
	virtual void data_bus(const uint8_t data) = 0;

	virtual void cpu_address_bus(const uint8_t data) = 0;
	virtual uint8_t cpu_data_bus() const = 0;
	virtual void cpu_data_bus(const uint8_t data) = 0;

	virtual void cpu_read() = 0;
	virtual void cpu_write() = 0;

	virtual bool read_write() const = 0;
	virtual uint32_t vout() const = 0;

	virtual void oam_dma(const uint8_t data) = 0;
};
