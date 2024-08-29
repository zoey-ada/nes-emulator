#pragma once

#include <string>
#include <vector>

#include <base/pixel.hpp>
#include <cpu/addressing_modes.hpp>
#include <cpu/cpu.hpp>

#include "cpuRecorder.hpp"
#include "cpuRenderer.hpp"
#include "cpuStackFrame.hpp"
#include "decompiler.hpp"

class IMemory;
struct SDL_Renderer;
struct SDL_Texture;

class DebugCpu: public Cpu
{
public:
	DebugCpu(IMemory* memory, SDL_Renderer* renderer);
	virtual ~DebugCpu() = default;

	void reset() override;
	void nmi_impl() override;
	void irq_impl() override;

	void cycle() override;
	void cycle(uint64_t number_cycles) override;

	SDL_Texture* getTexture();

	inline CpuStackFrame getLastStackFrame() const { return this->_last_cycle; }

private:
	Decompiler _decompiler;
	CpuRecorder _recorder;
	CpuRenderer _renderer;

	CpuStackFrame _last_cycle;
	std::string _current_instruction;
	bool _cycle_is_new_instruction {false};

	bool _record_stack_frames {false};

	// Cartridge* _cart;

	void queue_next_instruction() override;
};
