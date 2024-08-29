#pragma once

#include <cstdint>
#include <deque>
#include <functional>

#include <base/iDma.hpp>

class IApu;
class ICpu;
class IMemory;
class IPpu;

using Action = std::function<void()>;

const uint16_t oam_length = 256;

class DirectMemoryAccess: public IDma
{
public:
	DirectMemoryAccess() = default;
	virtual ~DirectMemoryAccess();

	void initialize(ICpu* cpu, IMemory* cpu_memory, IMemory* oam_memory);

	void cycle() override;

	void copyToOam(const uint16_t address) override;

private:
	ICpu* _cpu {nullptr};
	IPpu* _ppu {nullptr};
	IMemory* _cpu_memory {nullptr};
	IMemory* _oam_memory {nullptr};

	bool _is_read_cycle {true};
	bool _performing_operation {true};

	std::deque<Action> _actions;

	uint8_t _data_latch;
	uint16_t _read_address_latch;
	uint8_t _address_offset;
};
