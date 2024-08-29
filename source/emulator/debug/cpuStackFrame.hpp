#pragma once

#include <cstdint>
#include <string>

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
