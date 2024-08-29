#include "debugCpu.hpp"

#include <format>

#include <base/iMemory.hpp>

#include "operations.hpp"

// void DebugCpu::produceFrame()
// {}

DebugCpu::DebugCpu(IMemory* memory, SDL_Renderer* renderer)
	: Cpu(memory), _decompiler(memory), _recorder(), _renderer(renderer)
{}

void DebugCpu::reset()
{
	this->_current_instruction = "RES         ";

	Cpu::reset();
}

void DebugCpu::nmi_impl()
{
	this->_current_instruction = "NMI         ";

	Cpu::nmi_impl();
}

void DebugCpu::irq_impl()
{
	this->_current_instruction = "IRQ         ";

	Cpu::irq_impl();
}

void DebugCpu::cycle()
{
	Cpu::cycle();

	this->_last_cycle = {
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

	if (this->_record_stack_frames)
		this->_recorder.logFrame(this->_last_cycle);
}

void DebugCpu::cycle(const uint64_t number_cycles)
{
	for (auto i = 0; i < number_cycles; ++i)
		this->cycle();
}

SDL_Texture* DebugCpu::getTexture()
{
	this->_renderer.produceFrame(this->_last_cycle);
	return this->_renderer.getTexture();
}

void DebugCpu::queue_next_instruction()
{
	this->_current_instruction = this->_decompiler.decompileInstruction(this->program_counter());
	Cpu::queue_next_instruction();
}
