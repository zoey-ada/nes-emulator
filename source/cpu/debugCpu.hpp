#pragma once

#include <string>
#include <vector>

#include <base/pixel.hpp>

#include "addressing_modes.hpp"
#include "cpu.hpp"

struct CpuStackFrame
{
	uint8_t a {0x00};
	uint8_t x {0x00};
	uint8_t y {0x00};
	uint8_t p {0x00};
	uint8_t s {0x00};
	uint16_t program_counter {0x0000};
	uint8_t opcode {0x00};
	uint8_t input_data_latch {0x00};
	uint16_t address_bus {0x0000};
	uint8_t data_bus {0x00};
	std::string instruction;
};

class DebugCpu: public Cpu
{
public:
	explicit DebugCpu(IMemory* memory);
	virtual ~DebugCpu() = default;

	void power_up();

	void cycle();
	void cycle(uint8_t number_cycles);

	CpuStackFrame getLastStackFrame();
	CpuStackFrame getStackFrame(uint64_t frame_number);

private:
	// CpuFrame _frame;
	std::vector<CpuStackFrame> _stack_frame_log;
	uint64_t _cycle_number {UINT64_MAX};
	std::string _current_instruction;
	std::map<AddressingMode, uint8_t> _bytes_to_read;
	bool _cycle_is_new_instruction {false};

	void queue_next_instruction() override;
	void grab_new_instruction();
};
