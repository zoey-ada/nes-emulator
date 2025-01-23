#pragma once

#include <string>
#include <vector>

#include <base/pixel.hpp>
#include <cpu/addressing_modes.hpp>
#include <cpu/cpu.hpp>
#include <platform/iRenderer.hpp>

#include "cpuRecorder.hpp"
#include "cpuRenderer.hpp"
#include "cpuStackFrame.hpp"
#include "decompiler.hpp"

class IMemory;

class DebugCpu: public Cpu
{
public:
	DebugCpu(IMemory* memory, std::shared_ptr<IRenderer> renderer);
	virtual ~DebugCpu() = default;

	void reset() override;
	void nmi_impl() override;
	void irq_impl() override;

	// void cycle() override;
	// void cycle(uint64_t number_cycles) override;

	Texture getTexture();

	inline CpuStackFrame getLastStackFrame() const { return this->_last_cycle; }

	void setDebugRenderer(IRenderer* renderer);

private:
	Decompiler _decompiler;
	// CodeViewer _code_viewer;
	CpuRecorder _cpu_recorder;
	CpuRenderer _cpu_renderer;

	CpuStackFrame _last_cycle;
	std::string _current_instruction;
	bool _cycle_is_new_instruction {false};

	bool _record_stack_frames {false};

	// Cartridge* _cart;

	// void queue_next_instruction() override;
};
