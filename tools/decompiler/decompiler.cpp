#include "decompiler.hpp"

#include <format>

#include <base/iMemory.hpp>

Decompiler::Decompiler(IMemory* program_memory): _program_memory(program_memory)
{}

Decompiler::~Decompiler()
{
	this->_program_memory = nullptr;
}

void Decompiler::loadProgramMemory(IMemory* program_memory)
{
	this->_program_memory = program_memory;
}

std::map<uint16_t, std::string> Decompiler::decompileAllInstructions(
	const uint16_t memory_size) const
{
	uint16_t address = 0;
	std::map<uint16_t, std::string> code;
	uint64_t index = 0;

	while (address < memory_size)
	{
		auto opcode = this->_program_memory->read(address);
		if (operations.contains(opcode))
		{
			auto operation = operations[opcode];
			code[index] =
				std::format("${:04x}  {}", address, this->decompileInstruction(operation, address));
			address += this->getNumBytes(operation.addressing_mode);
			index++;
		}
		else
			address++;
	}

	return code;
}

std::string Decompiler::decompileInstruction(const uint16_t address) const
{
	auto opcode = this->_program_memory->read(address);
	auto operation = operations[opcode];
	return this->decompileInstruction(operation, address);
}

std::string Decompiler::decompileInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	switch (operation.addressing_mode)
	{
	case AddressingMode::accumulator:
		return this->readAccumulatorInstruction(operation);
	case AddressingMode::implicit:
		return this->readImplicitInstruction(operation);
	case AddressingMode::immediate:
		return this->readImmediateInstruction(operation, address);
	case AddressingMode::relative:
		return this->readRelativeInstruction(operation, address);
	case AddressingMode::indirect:
		return this->readIndirectInstruction(operation, address);
	case AddressingMode::indirect_x_read:
	case AddressingMode::indirect_x_write:
		return this->readIndirectXInstruction(operation, address);
	case AddressingMode::indirect_y_read:
	case AddressingMode::indirect_y_write:
		return this->readIndirectYInstruction(operation, address);
	case AddressingMode::zero_page:
		return this->readZeroPageInstruction(operation, address);
	case AddressingMode::zero_page_x_read:
	case AddressingMode::zero_page_x_write:
		return this->readZeroPageXInstruction(operation, address);
	case AddressingMode::zero_page_y_read:
	case AddressingMode::zero_page_y_write:
		return this->readZeroPageYInstruction(operation, address);
	case AddressingMode::absolute:
		return this->readAbsoluteInstruction(operation, address);
	case AddressingMode::absolute_x_read:
	case AddressingMode::absolute_x_write:
		return this->readAbsoluteXInstruction(operation, address);
	case AddressingMode::absolute_y_read:
	case AddressingMode::absolute_y_write:
		return this->readAbsoluteYInstruction(operation, address);
	default:
		throw std::exception("this should be unreachable?");
	}
}

std::string Decompiler::readAccumulatorInstruction(const OperationInfo& operation) const
{
	char buffer[13];
	snprintf(buffer, 13, "%s A       ", operation.name.c_str());
	return std::string(buffer);
	// return std::format("{} A       ", operation.name);
}

std::string Decompiler::readImplicitInstruction(const OperationInfo& operation) const
{
	char buffer[13];
	snprintf(buffer, 13, "%s         ", operation.name.c_str());
	return std::string(buffer);
	// return std::format("{}         ", operation.name);
}

std::string Decompiler::readImmediateInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	uint8_t data = this->_program_memory->read(address + 1);

	char buffer[13];
	snprintf(buffer, 13, "%s #%02d     ", operation.name.c_str(), data);
	return std::string(buffer);
	// return std::format("{} #${:02x}    ", operation.name, data);
}

std::string Decompiler::readRelativeInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	int8_t data = this->asSigned(this->_program_memory->read(address + 1));

	char buffer[13];
	snprintf(buffer, 13, "%s *%+02d     ", operation.name.c_str(), data);
	return std::string(buffer);
	// return std::format("{} *${:+02x}    ", operation.name, data);
}

std::string Decompiler::readIndirectInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	uint8_t low_byte = this->_program_memory->read(address + 1);
	uint8_t high_byte = this->_program_memory->read(address + 2);

	char buffer[13];
	snprintf(buffer, 13, "%s ($%02x%02x) ", operation.name.c_str(), high_byte, low_byte);
	return std::string(buffer);
	// return std::format("{} (${:02x}{:02x}) ", operation.name, data1, data2);
}

std::string Decompiler::readIndirectXInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	uint8_t data = this->_program_memory->read(address + 1);

	char buffer[13];
	snprintf(buffer, 13, "%s ($%02x,X) ", operation.name.c_str(), data);
	return std::string(buffer);
	// return std::format("{} (${:02x},X) ", operation.name, data);
}

std::string Decompiler::readIndirectYInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	uint8_t data = this->_program_memory->read(address + 1);

	char buffer[13];
	snprintf(buffer, 13, "%s ($%02x),Y ", operation.name.c_str(), data);
	return std::string(buffer);
	// return std::format("{} (${:02x}),Y ", operation.name, data);
}

std::string Decompiler::readZeroPageInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	uint8_t data = this->_program_memory->read(address + 1);

	char buffer[13];
	snprintf(buffer, 13, "%s $%02x     ", operation.name.c_str(), data);
	return std::string(buffer);
	// return std::format("{} ${:02x}     ", operation.name, data);
}

std::string Decompiler::readZeroPageXInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	uint8_t data = this->_program_memory->read(address + 1);

	char buffer[13];
	snprintf(buffer, 13, "%s $%02x,X   ", operation.name.c_str(), data);
	return std::string(buffer);
	// return std::format("{} ${:02x},X   ", operation.name, data);
}

std::string Decompiler::readZeroPageYInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	uint8_t data = this->_program_memory->read(address + 1);

	char buffer[13];
	snprintf(buffer, 13, "%s $%02x,Y   ", operation.name.c_str(), data);
	return std::string(buffer);
	// return std::format("{} ${:02x},Y   ", operation.name, data);
}

std::string Decompiler::readAbsoluteInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	uint8_t low_byte = this->_program_memory->read(address + 1);
	uint8_t high_byte = this->_program_memory->read(address + 2);

	char buffer[13];
	snprintf(buffer, 13, "%s $%02x%02x   ", operation.name.c_str(), high_byte, low_byte);
	return std::string(buffer);
	// return std::format("{} ${:02x}{:02x}   ", operation.name, data1, data2);
}

std::string Decompiler::readAbsoluteXInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	uint8_t low_byte = this->_program_memory->read(address + 1);
	uint8_t high_byte = this->_program_memory->read(address + 2);

	char buffer[13];
	snprintf(buffer, 13, "%s $%02x%02x,X ", operation.name.c_str(), high_byte, low_byte);
	return std::string(buffer);
	// return std::format("{} ${:02x}{:02x},X ", operation.name, data1, data2);
}

std::string Decompiler::readAbsoluteYInstruction(const OperationInfo& operation,
	const uint16_t address) const
{
	uint8_t low_byte = this->_program_memory->read(address + 1);
	uint8_t high_byte = this->_program_memory->read(address + 2);

	char buffer[13];
	snprintf(buffer, 13, "%s $%02x%02x,Y ", operation.name.c_str(), high_byte, low_byte);
	return std::string(buffer);
	// return std::format("{} ${:02x}{:02x},Y ", operation.name, data1, data2);
}

int8_t Decompiler::asSigned(const uint8_t data) const
{
	const int8_t signed_data = *(reinterpret_cast<const int8_t*>(&data));
	return signed_data;
}

uint8_t Decompiler::getNumBytes(const AddressingMode addressing_mode) const
{
	switch (addressing_mode)
	{
	case AddressingMode::accumulator:
	case AddressingMode::implicit:
		return 1;
	case AddressingMode::immediate:
	case AddressingMode::relative:
	case AddressingMode::indirect_x_read:
	case AddressingMode::indirect_x_write:
	case AddressingMode::indirect_y_read:
	case AddressingMode::indirect_y_write:
	case AddressingMode::zero_page:
	case AddressingMode::zero_page_x_read:
	case AddressingMode::zero_page_x_write:
	case AddressingMode::zero_page_y_read:
	case AddressingMode::zero_page_y_write:
		return 2;
	case AddressingMode::indirect:
	case AddressingMode::absolute:
	case AddressingMode::absolute_x_read:
	case AddressingMode::absolute_x_write:
	case AddressingMode::absolute_y_read:
	case AddressingMode::absolute_y_write:
		return 3;
	default:
		throw std::exception("this should be unreachable?");
	}
}
