#include "debugCpu.hpp"

#include <format>

#include <base/iMemory.hpp>
#include <platform/sdlRenderer.hpp>

#include "operations.hpp"

DebugCpu::DebugCpu(IMemory* memory, std::shared_ptr<IRenderer> renderer)
	: Cpu(memory), _decompiler(memory), _cpu_recorder(), _cpu_renderer(renderer)
{
	// std::make_shared<SdlRenderer>();
	// _code_viewer(renderer, 1)
}

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
		this->_accumulator(),
		this->_x_register(),
		this->_y_register(),
		this->_status_register(),
		this->_stack_register(),
		this->_program_counter(),
		this->instruction_register(),
		this->input_data_latch(),
		this->address_bus(),
		this->data_bus(),
		this->_current_instruction,
	};

	if (this->_record_stack_frames)
		this->_cpu_recorder.logFrame(this->_last_cycle);
}

void DebugCpu::cycle(const uint64_t number_cycles)
{
	for (auto i = 0; i < number_cycles; ++i)
		this->cycle();
}

Texture DebugCpu::getTexture()
{
	this->_cpu_renderer.produceFrame(this->_last_cycle);
	return this->_cpu_renderer.getTexture();
}

void DebugCpu::queue_next_instruction()
{
	this->_current_instruction = this->_decompiler.decompileInstruction(this->_program_counter());
	Cpu::queue_next_instruction();
}
