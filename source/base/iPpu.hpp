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

	virtual bool read_write() const = 0;
	virtual uint32_t vout() const = 0;

	virtual void ppu_ctrl(const uint8_t value) = 0;
	virtual inline void ppu_mask(const uint8_t value) = 0;
	virtual uint8_t ppu_status() = 0;
	virtual inline void oam_addr(const uint8_t value) = 0;
	virtual inline uint8_t oam_data() const = 0;
	virtual inline void oam_data(const uint8_t value) = 0;
	virtual void ppu_scroll(const uint8_t value) = 0;
	virtual void ppu_addr(const uint8_t value) = 0;
	virtual uint8_t ppu_data() = 0;
	virtual void ppu_data(const uint8_t value) = 0;
	virtual inline void oam_dma(const uint8_t value) = 0;
};
