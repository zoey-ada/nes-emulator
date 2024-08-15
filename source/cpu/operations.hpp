#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "addressing_modes.hpp"

struct OperationInfo
{
	uint8_t opcode;
	std::string name;
	AddressingMode addressing_mode;
	uint8_t cycles;
};

extern std::map<uint8_t, OperationInfo> operations;
