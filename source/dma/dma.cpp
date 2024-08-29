#include "dma.hpp"

#include <cassert>

#include <base/iCpu.hpp>
#include <base/iMemory.hpp>
#include <base/iPpu.hpp>

DirectMemoryAccess::~DirectMemoryAccess()
{
	this->_cpu = nullptr;
	this->_cpu_memory = nullptr;
	this->_oam_memory = nullptr;
}

void DirectMemoryAccess::initialize(ICpu* cpu, IMemory* cpu_memory, IMemory* oam_memory)
{
	this->_cpu = cpu;
	this->_cpu_memory = cpu_memory;
	this->_oam_memory = oam_memory;

	assert(this->_cpu);
	assert(this->_cpu_memory);
	assert(this->_oam_memory);
}

void DirectMemoryAccess::cycle()
{
	if (this->_performing_operation)
	{
		if (this->_actions.empty())
		{
			this->_performing_operation = false;
		}
		else
		{
			auto action = this->_actions.front();
			this->_actions.pop_front();
			action();
		}
	}
	else if (this->_is_read_cycle && !this->_actions.empty())
	{
		this->_performing_operation = true;
		auto action = this->_actions.front();
		this->_actions.pop_front();
		action();
	}

	this->_is_read_cycle = !this->_is_read_cycle;
}

void DirectMemoryAccess::copyToOam(const uint16_t address)
{
	this->_read_address_latch = address;
	this->_address_offset = 0x00;
	this->_cpu->suspend();

	for (auto i = 0; i < oam_length - 1; ++i)
	{
		this->_actions.push_back([this] {
			uint16_t mem_addr = this->_read_address_latch + this->_address_offset;
			this->_data_latch = this->_cpu_memory->read(mem_addr);
		});
		this->_actions.push_back([this] {
			this->_oam_memory->write(this->_address_offset, this->_data_latch);
			this->_address_offset++;
		});
	}

	this->_actions.push_back([this] {
		uint16_t mem_addr = this->_read_address_latch + this->_address_offset;
		this->_data_latch = this->_cpu_memory->read(mem_addr);
	});
	this->_actions.push_back([this] {
		this->_oam_memory->write(this->_address_offset, this->_data_latch);
		this->_address_offset++;
		this->_cpu->activate();
	});
}
