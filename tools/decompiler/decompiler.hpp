#pragma once

#include <map>
#include <string>

#include "operations.hpp"

class IMemory;

class Decompiler
{
public:
	Decompiler(IMemory* program_memory);
	virtual ~Decompiler();

	void loadProgramMemory(IMemory* program_memory);

	std::map<uint16_t, std::string> decompileAllInstructions(const uint16_t memory_size) const;
	std::string decompileInstruction(const uint16_t address) const;
	std::string decompileInstruction(const OperationInfo& operation, const uint16_t address) const;

private:
	IMemory* _program_memory {nullptr};

	std::string readAccumulatorInstruction(const OperationInfo& operation) const;
	std::string readImplicitInstruction(const OperationInfo& operation) const;
	std::string readImmediateInstruction(const OperationInfo& operation,
		const uint16_t address) const;
	std::string readRelativeInstruction(const OperationInfo& operation,
		const uint16_t address) const;

	std::string readIndirectInstruction(const OperationInfo& operation,
		const uint16_t address) const;
	std::string readIndirectXInstruction(const OperationInfo& operation,
		const uint16_t address) const;
	std::string readIndirectYInstruction(const OperationInfo& operation,
		const uint16_t address) const;

	std::string readZeroPageInstruction(const OperationInfo& operation,
		const uint16_t address) const;
	std::string readZeroPageXInstruction(const OperationInfo& operation,
		const uint16_t address) const;
	std::string readZeroPageYInstruction(const OperationInfo& operation,
		const uint16_t address) const;

	std::string readAbsoluteInstruction(const OperationInfo& operation,
		const uint16_t address) const;
	std::string readAbsoluteXInstruction(const OperationInfo& operation,
		const uint16_t address) const;
	std::string readAbsoluteYInstruction(const OperationInfo& operation,
		const uint16_t address) const;

	int8_t asSigned(const uint8_t data) const;

	uint8_t getNumBytes(const AddressingMode addressing_mode) const;
};
