#include "debugCpu.hpp"

#include <format>

#include <memory/iMemory.hpp>

#include "operations.hpp"

// void DebugCpu::produceFrame()
// {}

DebugCpu::DebugCpu(IMemory* memory): Cpu(memory)
{
	this->_bytes_to_read = {
		{AddressingMode::zero_page, 2},
		{AddressingMode::zero_page_x_read, 2},
		{AddressingMode::zero_page_x_write, 2},
		{AddressingMode::zero_page_y_read, 2},
		{AddressingMode::zero_page_y_write, 2},
		{AddressingMode::absolute, 3},
		{AddressingMode::absolute_x_read, 3},
		{AddressingMode::absolute_x_write, 3},
		{AddressingMode::absolute_y_read, 3},
		{AddressingMode::absolute_y_write, 3},
		{AddressingMode::indirect, 3},
		{AddressingMode::indirect_x_read, 2},
		{AddressingMode::indirect_x_write, 2},
		{AddressingMode::indirect_y_read, 2},
		{AddressingMode::indirect_y_write, 2},
		{AddressingMode::relative, 2},
		{AddressingMode::immediate, 2},
		{AddressingMode::accumulator, 1},
		{AddressingMode::implicit, 1},
	};
}

void DebugCpu::power_up()
{
	this->_stack_frame_log.clear();
	this->_cycle_number = UINT64_MAX;
	this->_current_instruction = "RES";
	Cpu::power_up();
}

void DebugCpu::cycle()
{
	Cpu::cycle();

	if (this->_cycle_is_new_instruction)
		this->grab_new_instruction();

	CpuStackFrame current {
		this->accumulator(),
		this->x_register(),
		this->y_register(),
		this->status_register(),
		this->stack_register(),
		this->program_counter(),
		this->instruction_register(),
		this->input_data_latch(),
		this->address_bus(),
		this->data_bus(),
		this->_current_instruction,
	};

	this->_stack_frame_log.push_back(current);
	++this->_cycle_number;
}

void DebugCpu::cycle(const uint8_t number_cycles)
{
	for (auto i = 0; i < number_cycles; ++i)
		this->cycle();
}

CpuStackFrame DebugCpu::getLastStackFrame()
{
	return this->getStackFrame(this->_cycle_number);
}

CpuStackFrame DebugCpu::getStackFrame(uint64_t frame_number)
{
	if (frame_number == UINT64_MAX)
		return {};
	if (this->_stack_frame_log.size() < (frame_number + 1))
		return {};

	return this->_stack_frame_log[frame_number];
}

void DebugCpu::queue_next_instruction()
{
	Cpu::queue_next_instruction();
	this->_cycle_is_new_instruction = true;
}

void DebugCpu::grab_new_instruction()
{
	auto operation = operations[this->instruction_register()];
	auto num_instruction_bytes = this->_bytes_to_read[operation.addressing_mode];

	// {AddressingMode::indirect, 3},
	switch (num_instruction_bytes)
	{
	case 1:
	{
		auto a_symbol = operation.addressing_mode == AddressingMode::accumulator ? "A" : "";
		this->_current_instruction = std::format("{} {}       ", operation.name, a_symbol);
		break;
	}
	case 2:
	{
		auto op_byte_2 = this->_memory->read(this->program_counter());

		if (operation.addressing_mode == AddressingMode::immediate)
		{
			this->_current_instruction = std::format("{} #{}      ", operation.name, op_byte_2);
		}
		else if (operation.addressing_mode == AddressingMode::relative)
		{
			auto signed_op_byte_2 = *(reinterpret_cast<int8_t*>(&op_byte_2));
			this->_current_instruction =
				std::format("{} *{:+}  ", operation.name, signed_op_byte_2);
		}
		else if (operation.addressing_mode == AddressingMode::indirect_x_read ||
			operation.addressing_mode == AddressingMode::indirect_x_write)
		{
			this->_current_instruction = std::format("{} (${:02x},X)  ", operation.name, op_byte_2);
		}
		else if (operation.addressing_mode == AddressingMode::indirect_y_read ||
			operation.addressing_mode == AddressingMode::indirect_y_write)
		{
			this->_current_instruction =
				std::format("{} (${:02x}),Y   ", operation.name, op_byte_2);
		}
		else
		{
			std::string index_symbol = "";
			if (operation.addressing_mode == AddressingMode::zero_page_x_read ||
				operation.addressing_mode == AddressingMode::zero_page_x_write)
				index_symbol = ",X";
			else if (operation.addressing_mode == AddressingMode::zero_page_y_read ||
				operation.addressing_mode == AddressingMode::zero_page_y_read)
				index_symbol = ",Y";

			this->_current_instruction =
				std::format("{} ${:02x}{}    ", operation.name, op_byte_2, index_symbol);
		}
		break;
	}
	case 3:
	{
		auto op_byte_2 = this->_memory->read(this->program_counter());
		auto op_byte_3 = this->_memory->read(this->program_counter() + 1);

		if (operation.addressing_mode == AddressingMode::indirect)
		{
			this->_current_instruction =
				std::format("{} (${:02x}{:02x})  ", operation.name, op_byte_3, op_byte_2);
		}
		else
		{
			std::string index_symbol = "";
			if (operation.addressing_mode == AddressingMode::absolute_x_read ||
				operation.addressing_mode == AddressingMode::absolute_x_write)
				index_symbol = ",X";
			else if (operation.addressing_mode == AddressingMode::absolute_y_read ||
				operation.addressing_mode == AddressingMode::absolute_y_read)
				index_symbol = ",Y";

			this->_current_instruction = std::format("{} ${:02x}{:02x}{}  ", operation.name,
				op_byte_3, op_byte_2, index_symbol);
		}
		break;
	}
	}

	this->_cycle_is_new_instruction = false;
}
