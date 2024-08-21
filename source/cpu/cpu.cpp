#include "cpu.hpp"

#include <cassert>

#include <base/iMemory.hpp>

Cpu::Cpu(IMemory* memory)
{
	assert(memory != nullptr);
	this->_memory = memory;

	this->populate_operations();
	this->populate_addressing_modes();
}

Cpu::~Cpu()
{
	this->_memory = nullptr;
}

void Cpu::clear_registers()
{
	accumulator(0x00);
	x_register(0x00);
	y_register(0x00);
	program_counter(0xfffc);
	stack_register(0xfd);
	c_flag(0);
	z_flag(0);
	i_flag(1);
	d_flag(0);
	v_flag(0);
	n_flag(0);
}

void Cpu::reset()
{
	this->clear_registers();
	this->_actions.clear();
	this->res(AddressingMode::implicit);
}

void Cpu::cycle()
{
	if (this->_actions.empty())
		this->queue_next_instruction();

	auto action = this->_actions.front();
	this->_actions.pop_front();
	action();
}

void Cpu::cycle(const uint64_t number_of_cycles)
{
	for (auto i = 0; i < number_of_cycles; ++i)
		this->cycle();
}

void Cpu::fetch()
{
	this->_address_bus(this->program_counter());
	this->read_memory();
	this->program_counter()++;
}

void Cpu::read_memory()
{
	this->_read_write = true;
	this->_data_bus(this->_memory->read(this->address_bus()));
}

void Cpu::write_memory()
{
	this->_read_write = false;
	this->_memory->write(this->address_bus(), this->_data_bus());
}

void Cpu::nmi()
{
	this->nmi(AddressingMode::implicit);
}

void Cpu::irq()
{
	this->irq(AddressingMode::implicit);
}

void Cpu::queue_next_instruction()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_instruction = this->_data_bus();
		auto operation = this->_operations.at(this->_instruction);
		std::invoke(operation.operation, *this, operation.addressing_mode);
	});
}

void Cpu::populate_addressing_modes()
{
	this->_addressing_modes = {
		{AddressingMode::accumulator, &Cpu::no_address},
		{AddressingMode::implicit, &Cpu::no_address},
		{AddressingMode::immediate, &Cpu::no_address},
		{AddressingMode::relative, &Cpu::no_address},
		{AddressingMode::zero_page, &Cpu::zero_page},
		{AddressingMode::zero_page_x_read, &Cpu::zero_page_x},
		{AddressingMode::zero_page_x_write, &Cpu::zero_page_x},
		{AddressingMode::zero_page_y_read, &Cpu::zero_page_y},
		{AddressingMode::zero_page_y_write, &Cpu::zero_page_y},
		{AddressingMode::absolute, &Cpu::absolute},
		{AddressingMode::absolute_x_read, &Cpu::absolute_x_read},
		{AddressingMode::absolute_x_write, &Cpu::absolute_x_write},
		{AddressingMode::absolute_y_read, &Cpu::absolute_y_read},
		{AddressingMode::absolute_y_write, &Cpu::absolute_y_write},
		{AddressingMode::indirect, &Cpu::indirect},
		{AddressingMode::indirect_x_read, &Cpu::indirect_x},
		{AddressingMode::indirect_x_write, &Cpu::indirect_x},
		{AddressingMode::indirect_y_read, &Cpu::indirect_y_read},
		{AddressingMode::indirect_y_write, &Cpu::indirect_y_write},
	};
}

void Cpu::populate_operations()
{
	this->_operations = {
		{0x00, {&Cpu::brk, AddressingMode::implicit}},
		{0x01, {&Cpu::ora, AddressingMode::indirect_x_read}},
		// {0x02, {&Cpu::stp, AddressingMode::implicit}},
		// {0x03, {&Cpu::nop, AddressingMode::implicit}},
		// {0x04, {&Cpu::nop, AddressingMode::implicit}},
		{0x05, {&Cpu::ora, AddressingMode::zero_page}},
		{0x06, {&Cpu::asl, AddressingMode::zero_page}},
		// {0x07, {&Cpu::nop, AddressingMode::implicit}},
		{0x08, {&Cpu::php, AddressingMode::implicit}},
		{0x09, {&Cpu::ora, AddressingMode::immediate}},
		{0x0a, {&Cpu::asl, AddressingMode::accumulator}},
		// {0x0b, {&Cpu::nop, AddressingMode::implicit}},
		// {0x0c, {&Cpu::nop, AddressingMode::implicit}},
		{0x0d, {&Cpu::ora, AddressingMode::absolute}},
		{0x0e, {&Cpu::asl, AddressingMode::absolute}},
		// {0x0f, {&Cpu::nop, AddressingMode::implicit}},
		{0x10, {&Cpu::bpl, AddressingMode::relative}},
		{0x11, {&Cpu::ora, AddressingMode::indirect_y_read}},
		// {0x12, {&Cpu::stp, AddressingMode::implicit}},
		// {0x13, {&Cpu::nop, AddressingMode::implicit}},
		// {0x14, {&Cpu::nop, AddressingMode::implicit}},
		{0x15, {&Cpu::ora, AddressingMode::zero_page_x_read}},
		{0x16, {&Cpu::asl, AddressingMode::zero_page_x_read}},
		// {0x17, {&Cpu::nop, AddressingMode::implicit}},
		{0x18, {&Cpu::clc, AddressingMode::implicit}},
		{0x19, {&Cpu::ora, AddressingMode::absolute_y_read}},
		// {0x1a, {&Cpu::nop, AddressingMode::implicit}},
		// {0x1b, {&Cpu::nop, AddressingMode::implicit}},
		// {0x1c, {&Cpu::nop, AddressingMode::implicit}},
		{0x1d, {&Cpu::ora, AddressingMode::absolute_x_read}},
		{0x1e, {&Cpu::asl, AddressingMode::absolute_x_write}},
		// {0x1f, {&Cpu::nop, AddressingMode::implicit}},
		{0x20, {&Cpu::jsr, AddressingMode::absolute}},
		{0x21, {&Cpu::and_, AddressingMode::indirect_x_read}},
		// {0x22, {&Cpu::nop, AddressingMode::implicit}},
		// {0x23, {&Cpu::nop, AddressingMode::implicit}},
		{0x24, {&Cpu::bit, AddressingMode::zero_page}},
		{0x25, {&Cpu::and_, AddressingMode::zero_page}},
		{0x26, {&Cpu::rol, AddressingMode::zero_page}},
		// {0x27, {&Cpu::nop, AddressingMode::implicit}},
		{0x28, {&Cpu::plp, AddressingMode::implicit}},
		{0x29, {&Cpu::and_, AddressingMode::immediate}},
		{0x2a, {&Cpu::rol, AddressingMode::accumulator}},
		// {0x2b, {&Cpu::nop, AddressingMode::implicit}},
		{0x2c, {&Cpu::bit, AddressingMode::absolute}},
		{0x2d, {&Cpu::and_, AddressingMode::absolute}},
		{0x2e, {&Cpu::rol, AddressingMode::absolute}},
		// {0x2f, {&Cpu::nop, AddressingMode::implicit}},
		{0x30, {&Cpu::bmi, AddressingMode::relative}},
		{0x31, {&Cpu::and_, AddressingMode::indirect_y_read}},
		// {0x32, {&Cpu::nop, AddressingMode::implicit}},
		// {0x33, {&Cpu::nop, AddressingMode::implicit}},
		// {0x34, {&Cpu::nop, AddressingMode::implicit}},
		{0x35, {&Cpu::and_, AddressingMode::zero_page_x_read}},
		{0x36, {&Cpu::rol, AddressingMode::zero_page_x_read}},
		// {0x37, {&Cpu::nop, AddressingMode::implicit}},
		{0x38, {&Cpu::sec, AddressingMode::implicit}},
		{0x39, {&Cpu::and_, AddressingMode::absolute_y_read}},
		// {0x3a, {&Cpu::nop, AddressingMode::implicit}},
		// {0x3b, {&Cpu::nop, AddressingMode::implicit}},
		// {0x3c, {&Cpu::nop, AddressingMode::implicit}},
		{0x3d, {&Cpu::and_, AddressingMode::absolute_x_read}},
		{0x3e, {&Cpu::rol, AddressingMode::absolute_x_write}},
		// {0x3f, {&Cpu::nop, AddressingMode::implicit}},
		{0x40, {&Cpu::rti, AddressingMode::implicit}},
		{0x41, {&Cpu::eor, AddressingMode::indirect_x_read}},
		// {0x42, {&Cpu::nop, AddressingMode::implicit}},
		// {0x43, {&Cpu::nop, AddressingMode::implicit}},
		// {0x44, {&Cpu::nop, AddressingMode::implicit}},
		{0x45, {&Cpu::eor, AddressingMode::zero_page}},
		{0x46, {&Cpu::lsr, AddressingMode::zero_page}},
		// {0x47, {&Cpu::nop, AddressingMode::implicit}},
		{0x48, {&Cpu::pha, AddressingMode::implicit}},
		{0x49, {&Cpu::eor, AddressingMode::immediate}},
		{0x4a, {&Cpu::lsr, AddressingMode::accumulator}},
		// {0x4b, {&Cpu::nop, AddressingMode::implicit}},
		{0x4c, {&Cpu::jmp, AddressingMode::absolute}},
		{0x4d, {&Cpu::eor, AddressingMode::absolute}},
		{0x4e, {&Cpu::lsr, AddressingMode::absolute}},
		// {0x4f, {&Cpu::nop, AddressingMode::implicit}},
		{0x50, {&Cpu::bvc, AddressingMode::relative}},
		{0x51, {&Cpu::eor, AddressingMode::indirect_y_read}},
		// {0x52, {&Cpu::nop, AddressingMode::implicit}},
		// {0x53, {&Cpu::nop, AddressingMode::implicit}},
		// {0x54, {&Cpu::nop, AddressingMode::implicit}},
		{0x55, {&Cpu::eor, AddressingMode::zero_page_x_read}},
		{0x56, {&Cpu::lsr, AddressingMode::zero_page_x_read}},
		// {0x57, {&Cpu::nop, AddressingMode::implicit}},
		{0x58, {&Cpu::cli, AddressingMode::implicit}},
		{0x59, {&Cpu::eor, AddressingMode::absolute_y_read}},
		// {0x5a, {&Cpu::nop, AddressingMode::implicit}},
		// {0x5b, {&Cpu::nop, AddressingMode::implicit}},
		// {0x5c, {&Cpu::nop, AddressingMode::implicit}},
		{0x5d, {&Cpu::eor, AddressingMode::absolute_x_read}},
		{0x5e, {&Cpu::lsr, AddressingMode::absolute_x_write}},
		// {0x5f, {&Cpu::nop, AddressingMode::implicit}},
		{0x60, {&Cpu::rts, AddressingMode::implicit}},
		{0x61, {&Cpu::adc, AddressingMode::indirect_x_read}},
		// {0x62, {&Cpu::nop, AddressingMode::implicit}},
		// {0x63, {&Cpu::nop, AddressingMode::implicit}},
		// {0x64, {&Cpu::nop, AddressingMode::implicit}},
		{0x65, {&Cpu::adc, AddressingMode::zero_page}},
		{0x66, {&Cpu::ror, AddressingMode::zero_page}},
		// {0x67, {&Cpu::nop, AddressingMode::implicit}},
		{0x68, {&Cpu::pla, AddressingMode::implicit}},
		{0x69, {&Cpu::adc, AddressingMode::immediate}},
		{0x6a, {&Cpu::ror, AddressingMode::accumulator}},
		// {0x6b, {&Cpu::nop, AddressingMode::implicit}},
		{0x6c, {&Cpu::jmp, AddressingMode::indirect}},
		{0x6d, {&Cpu::adc, AddressingMode::absolute}},
		{0x6e, {&Cpu::ror, AddressingMode::absolute}},
		// {0x6f, {&Cpu::nop, AddressingMode::implicit}},
		{0x70, {&Cpu::bvs, AddressingMode::relative}},
		{0x71, {&Cpu::adc, AddressingMode::indirect_y_read}},
		// {0x72, {&Cpu::nop, AddressingMode::implicit}},
		// {0x73, {&Cpu::nop, AddressingMode::implicit}},
		// {0x74, {&Cpu::nop, AddressingMode::implicit}},
		{0x75, {&Cpu::adc, AddressingMode::zero_page_x_read}},
		{0x76, {&Cpu::ror, AddressingMode::zero_page_x_read}},
		// {0x77, {&Cpu::nop, AddressingMode::implicit}},
		{0x78, {&Cpu::sei, AddressingMode::implicit}},
		{0x79, {&Cpu::adc, AddressingMode::absolute_y_read}},
		// {0x7a, {&Cpu::nop, AddressingMode::implicit}},
		// {0x7b, {&Cpu::nop, AddressingMode::implicit}},
		// {0x7c, {&Cpu::nop, AddressingMode::implicit}},
		{0x7d, {&Cpu::adc, AddressingMode::absolute_x_read}},
		{0x7e, {&Cpu::ror, AddressingMode::absolute_x_write}},
		// {0x7f, {&Cpu::nop, AddressingMode::implicit}},
		// {0x80, {&Cpu::nop, AddressingMode::implicit}},
		{0x81, {&Cpu::sta, AddressingMode::indirect_x_write}},
		// {0x82, {&Cpu::nop, AddressingMode::implicit}},
		// {0x83, {&Cpu::nop, AddressingMode::implicit}},
		{0x84, {&Cpu::sty, AddressingMode::zero_page}},
		{0x85, {&Cpu::sta, AddressingMode::zero_page}},
		{0x86, {&Cpu::stx, AddressingMode::zero_page}},
		// {0x87, {&Cpu::nop, AddressingMode::implicit}},
		{0x88, {&Cpu::dey, AddressingMode::implicit}},
		// {0x89, {&Cpu::nop, AddressingMode::implicit}},
		{0x8a, {&Cpu::txa, AddressingMode::implicit}},
		// {0x8b, {&Cpu::nop, AddressingMode::implicit}},
		{0x8c, {&Cpu::sty, AddressingMode::absolute}},
		{0x8d, {&Cpu::sta, AddressingMode::absolute}},
		{0x8e, {&Cpu::stx, AddressingMode::absolute}},
		// {0x8f, {&Cpu::nop, AddressingMode::implicit}},
		{0x90, {&Cpu::bcc, AddressingMode::relative}},
		{0x91, {&Cpu::sta, AddressingMode::indirect_y_write}},
		// {0x92, {&Cpu::nop, AddressingMode::implicit}},
		// {0x93, {&Cpu::nop, AddressingMode::implicit}},
		{0x94, {&Cpu::sty, AddressingMode::zero_page_x_write}},
		{0x95, {&Cpu::sta, AddressingMode::zero_page_x_write}},
		{0x96, {&Cpu::stx, AddressingMode::zero_page_y_write}},
		// {0x97, {&Cpu::nop, AddressingMode::implicit}},
		{0x98, {&Cpu::tya, AddressingMode::implicit}},
		{0x99, {&Cpu::sta, AddressingMode::absolute_y_write}},
		{0x9a, {&Cpu::txs, AddressingMode::implicit}},
		// {0x9b, {&Cpu::nop, AddressingMode::implicit}},
		// {0x9c, {&Cpu::nop, AddressingMode::implicit}},
		{0x9d, {&Cpu::sta, AddressingMode::absolute_x_write}},
		// {0x9e, {&Cpu::nop, AddressingMode::implicit}},
		// {0x9f, {&Cpu::nop, AddressingMode::implicit}},
		{0xa0, {&Cpu::ldy, AddressingMode::immediate}},
		{0xa1, {&Cpu::lda, AddressingMode::indirect_x_read}},
		{0xa2, {&Cpu::ldx, AddressingMode::immediate}},
		// {0xa3, {&Cpu::nop, AddressingMode::implicit}},
		{0xa4, {&Cpu::ldy, AddressingMode::zero_page}},
		{0xa5, {&Cpu::lda, AddressingMode::zero_page}},
		{0xa6, {&Cpu::ldx, AddressingMode::zero_page}},
		// {0xa7, {&Cpu::nop, AddressingMode::implicit}},
		{0xa8, {&Cpu::tay, AddressingMode::implicit}},
		{0xa9, {&Cpu::lda, AddressingMode::immediate}},
		{0xaa, {&Cpu::tax, AddressingMode::implicit}},
		// {0xab, {&Cpu::nop, AddressingMode::implicit}},
		{0xac, {&Cpu::ldy, AddressingMode::absolute}},
		{0xad, {&Cpu::lda, AddressingMode::absolute}},
		{0xae, {&Cpu::ldx, AddressingMode::absolute}},
		// {0xaf, {&Cpu::nop, AddressingMode::implicit}},
		{0xb0, {&Cpu::bcs, AddressingMode::relative}},
		{0xb1, {&Cpu::lda, AddressingMode::indirect_y_read}},
		// {0xb2, {&Cpu::nop, AddressingMode::implicit}},
		// {0xb3, {&Cpu::nop, AddressingMode::implicit}},
		{0xb4, {&Cpu::ldy, AddressingMode::zero_page_x_read}},
		{0xb5, {&Cpu::lda, AddressingMode::zero_page_x_read}},
		{0xb6, {&Cpu::ldx, AddressingMode::zero_page_y_read}},
		// {0xb7, {&Cpu::nop, AddressingMode::implicit}},
		{0xb8, {&Cpu::clv, AddressingMode::implicit}},
		{0xb9, {&Cpu::lda, AddressingMode::absolute_y_read}},
		{0xba, {&Cpu::tsx, AddressingMode::implicit}},
		// {0xbb, {&Cpu::nop, AddressingMode::implicit}},
		{0xbc, {&Cpu::ldy, AddressingMode::absolute_x_read}},
		{0xbd, {&Cpu::lda, AddressingMode::absolute_x_read}},
		{0xbe, {&Cpu::ldx, AddressingMode::absolute_y_read}},
		// {0xbf, {&Cpu::nop, AddressingMode::implicit}},
		{0xc0, {&Cpu::cpy, AddressingMode::immediate}},
		{0xc1, {&Cpu::cmp, AddressingMode::indirect_x_read}},
		// {0xc2, {&Cpu::nop, AddressingMode::implicit}},
		// {0xc3, {&Cpu::nop, AddressingMode::implicit}},
		{0xc4, {&Cpu::cpy, AddressingMode::zero_page}},
		{0xc5, {&Cpu::cmp, AddressingMode::zero_page}},
		{0xc6, {&Cpu::dec, AddressingMode::zero_page}},
		// {0xc7, {&Cpu::nop, AddressingMode::implicit}},
		{0xc8, {&Cpu::iny, AddressingMode::implicit}},
		{0xc9, {&Cpu::cmp, AddressingMode::immediate}},
		{0xca, {&Cpu::dex, AddressingMode::implicit}},
		// {0xcb, {&Cpu::nop, AddressingMode::implicit}},
		{0xcc, {&Cpu::cpy, AddressingMode::absolute}},
		{0xcd, {&Cpu::cmp, AddressingMode::absolute}},
		{0xce, {&Cpu::dec, AddressingMode::absolute}},
		// {0xcf, {&Cpu::nop, AddressingMode::implicit}},
		{0xd0, {&Cpu::bne, AddressingMode::relative}},
		{0xd1, {&Cpu::cmp, AddressingMode::indirect_y_read}},
		// {0xd2, {&Cpu::nop, AddressingMode::implicit}},
		// {0xd3, {&Cpu::nop, AddressingMode::implicit}},
		// {0xd4, {&Cpu::nop, AddressingMode::implicit}},
		{0xd5, {&Cpu::cmp, AddressingMode::zero_page_x_read}},
		{0xd6, {&Cpu::dec, AddressingMode::zero_page_x_read}},
		// {0xd7, {&Cpu::nop, AddressingMode::implicit}},
		{0xd8, {&Cpu::cld, AddressingMode::implicit}},
		{0xd9, {&Cpu::cmp, AddressingMode::absolute_y_read}},
		// {0xda, {&Cpu::nop, AddressingMode::implicit}},
		// {0xdb, {&Cpu::nop, AddressingMode::implicit}},
		// {0xdc, {&Cpu::nop, AddressingMode::implicit}},
		{0xdd, {&Cpu::cmp, AddressingMode::absolute_x_read}},
		{0xde, {&Cpu::dec, AddressingMode::absolute_x_write}},
		// {0xdf, {&Cpu::nop, AddressingMode::implicit}},
		{0xe0, {&Cpu::cpx, AddressingMode::immediate}},
		{0xe1, {&Cpu::sbc, AddressingMode::indirect_x_read}},
		// {0xe2, {&Cpu::nop, AddressingMode::implicit}},
		// {0xe3, {&Cpu::nop, AddressingMode::implicit}},
		{0xe4, {&Cpu::cpx, AddressingMode::zero_page}},
		{0xe5, {&Cpu::sbc, AddressingMode::zero_page}},
		{0xe6, {&Cpu::inc, AddressingMode::zero_page}},
		// {0xe7, {&Cpu::nop, AddressingMode::implicit}},
		{0xe8, {&Cpu::inx, AddressingMode::implicit}},
		{0xe9, {&Cpu::sbc, AddressingMode::immediate}},
		{0xea, {&Cpu::nop, AddressingMode::implicit}},
		// {0xeb, {&Cpu::nop, AddressingMode::implicit}},
		{0xec, {&Cpu::cpx, AddressingMode::absolute}},
		{0xed, {&Cpu::sbc, AddressingMode::absolute}},
		{0xee, {&Cpu::inc, AddressingMode::absolute}},
		// {0xef, {&Cpu::nop, AddressingMode::implicit}},
		{0xf0, {&Cpu::beq, AddressingMode::relative}},
		{0xf1, {&Cpu::sbc, AddressingMode::indirect_y_read}},
		// {0xf2, {&Cpu::nop, AddressingMode::implicit}},
		// {0xf3, {&Cpu::nop, AddressingMode::implicit}},
		// {0xf4, {&Cpu::nop, AddressingMode::implicit}},
		{0xf5, {&Cpu::sbc, AddressingMode::zero_page_x_read}},
		{0xf6, {&Cpu::inc, AddressingMode::zero_page_x_write}},
		// {0xf7, {&Cpu::nop, AddressingMode::implicit}},
		{0xf8, {&Cpu::sed, AddressingMode::implicit}},
		{0xf9, {&Cpu::sbc, AddressingMode::absolute_y_read}},
		// {0xfa, {&Cpu::nop, AddressingMode::implicit}},
		// {0xfb, {&Cpu::nop, AddressingMode::implicit}},
		// {0xfc, {&Cpu::nop, AddressingMode::implicit}},
		{0xfd, {&Cpu::sbc, AddressingMode::absolute_x_read}},
		{0xfe, {&Cpu::inc, AddressingMode::absolute_x_write}},
		// {0xff, {&Cpu::nop, AddressingMode::implicit}},
	};
}

void Cpu::queue_addressing_actions(AddressingMode mode)
{
	auto addressing_mode_func = this->_addressing_modes[mode];
	std::invoke(addressing_mode_func, *this);
}

void Cpu::no_address()
{}

void Cpu::relative()
{
	this->_actions.push_back([this]() {
		if ((this->_data_bus() & 0b1000'0000) > 1)
		{
			uint8_t twos_comp = (~this->_data_bus()) + 1;

			this->program_counter_low() =
				this->_alu.subtract(this->program_counter_low(), twos_comp);

			if (!this->_alu.carry())
				this->_actions.push_front([this]() {
					this->read_memory();
					this->program_counter_high() =
						this->_alu.subtract(this->program_counter_high(), 1);
				});
		}
		else
		{
			this->program_counter_low() =
				this->_alu.add(this->program_counter_low(), this->_data_bus());

			if (this->_alu.carry())
				this->_actions.push_front([this]() {
					this->read_memory();
					this->program_counter_high() = this->_alu.add(this->program_counter_high(), 1);
				});
		}
	});
}

void Cpu::zero_page()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(0x00);
		this->_address_bus.lowByte(this->_data_bus());
	});
}

void Cpu::zero_page_x()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(0x00);
		this->_address_bus.lowByte(this->_data_bus());
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.lowByte(this->_alu.add(this->address_bus(), this->_x));
	});
}

void Cpu::zero_page_y()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(0x00);
		this->_address_bus.lowByte(this->_data_bus());
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.lowByte(this->_alu.add(this->address_bus(), this->_y));
	});
}

void Cpu::absolute()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_input_data_latch = this->_data_bus();
	});

	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.lowByte(this->_input_data_latch);
		this->_address_bus.highByte(this->_data_bus());
	});
}

void Cpu::absolute_x_read()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_input_data_latch = this->_data_bus();
	});

	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(this->_data_bus());
		this->_address_bus.lowByte(this->_alu.add(this->_input_data_latch, this->_x));

		if (this->_alu.carry())
			this->_actions.push_front([this]() {
				this->read_memory();
				this->_address_bus.highByte(this->_alu.add(this->_address_bus.highByte(), 1));
			});
	});
}

void Cpu::absolute_x_write()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_input_data_latch = this->_data_bus();
	});

	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(this->_data_bus());
		this->_address_bus.lowByte(this->_alu.add(this->_input_data_latch, this->_x));
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.highByte(
			this->_alu.add(this->_address_bus.highByte(), this->_alu.carry() ? 1 : 0));
	});
}

void Cpu::absolute_y_read()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_input_data_latch = this->_data_bus();
	});

	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(this->_data_bus());
		this->_address_bus.lowByte(this->_alu.add(this->_input_data_latch, this->_y));

		if (this->_alu.carry())
			this->_actions.push_front([this]() {
				this->read_memory();
				this->_address_bus.highByte(this->_alu.add(this->_address_bus.highByte(), 1));
			});
	});
}

void Cpu::absolute_y_write()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_input_data_latch = this->_data_bus();
	});

	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(this->_data_bus());
		this->_address_bus.lowByte(this->_alu.add(this->_input_data_latch, this->_y));
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.highByte(
			this->_alu.add(this->_address_bus.highByte(), this->_alu.carry() ? 1 : 0));
	});
}

void Cpu::indirect()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_input_data_latch = this->_data_bus();
	});

	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.lowByte(this->_input_data_latch);
		this->_address_bus.highByte(this->_data_bus());
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->program_counter_low() = this->_data_bus();
		this->_address_bus.lowByte(this->_address_bus.lowByte() + 1);
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->program_counter_high() = this->_data_bus();
	});
}

void Cpu::indirect_x()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(0x00);
		this->_address_bus.lowByte(this->_data_bus());
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.lowByte(this->_alu.add(this->_address_bus.lowByte(), this->_x));
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.lowByte(this->_alu.add(this->_address_bus.lowByte(), 1));
		this->_input_data_latch = this->_data_bus();
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.lowByte(this->_input_data_latch);
		this->_address_bus.highByte(this->_data_bus());
	});
}

void Cpu::indirect_y_read()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(0x00);
		this->_address_bus.lowByte(this->_data_bus());
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.lowByte(this->_alu.add(this->_address_bus.lowByte(), 1));
		this->_input_data_latch = this->_data_bus();
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.highByte(this->_data_bus());
		this->_address_bus.lowByte(this->_alu.add(_input_data_latch, this->_y));

		if (this->_alu.carry())
		{
			this->_actions.push_front([this]() {
				this->read_memory();
				this->_address_bus.highByte(this->_alu.add(this->_address_bus.highByte(), 1));
			});
		}
	});
}

void Cpu::indirect_y_write()
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(0x00);
		this->_address_bus.lowByte(this->_data_bus());
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.lowByte(this->_alu.add(this->_address_bus.lowByte(), 1));
		this->_input_data_latch = this->_data_bus();
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.highByte(this->_data_bus());
		this->_address_bus.lowByte(this->_alu.add(_input_data_latch, this->_y));
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.highByte(
			this->_alu.add(this->_address_bus.highByte(), this->_alu.carry() ? 1 : 0));
	});
}

/// @brief Add with Carry (2-6 cycles)
/// @param mode 0x69 Immediate;
/// 0x65 Zero Page;
/// 0x75 Zero Page,X;
/// 0x6d Absolute;
/// 0x7d Absolute,X;
/// 0x79 Absolute,Y;
/// 0x61 Indirect,X;
/// 0x71 Indirect,Y;
void Cpu::adc(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->_a = this->_alu.add(this->_a, this->_data_bus(), this->c_flag());
		this->c_flag(this->_alu.carry());
		this->v_flag(this->_alu.overflow());
		this->z_flag(this->_a == 0);
		this->n_flag((this->_a & 0b10000000) > 0);
	});
}

/// @brief Logical AND (2-6 cycles)
/// @param mode 0x29 Immediate;
/// 0x25 Zero Page;
/// 0x35 Zero Page,X;
/// 0x2d Absolute;
/// 0x3d Absolute,X;
/// 0x39 Absolute,Y;
/// 0x21 Indirect,X;
/// 0x31 Indirect,Y;
void Cpu::and_(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->_a &= this->_data_bus();
		this->z_flag(this->_a == 0);
		this->n_flag((this->_a & 0b10000000) > 0);
	});
}

/// @brief Arithmetic Shift Left (2-7 cycles)
/// @param mode 0x0a Accumulator;
/// 0x06 Zero Page;
/// 0x16 Zero Page,X;
/// 0x0e Absolute;
/// 0x1e Absolute,X;
void Cpu::asl(AddressingMode mode)
{
	if (mode == AddressingMode::accumulator)
	{
		this->_actions.push_back([this, &mode]() {
			this->_a = this->_alu.shift_left(this->_a);
			this->c_flag(this->_alu.carry());
			this->z_flag(this->_a == 0);
			this->n_flag((this->_a & 0b10000000) > 0);
		});
		return;
	}

	this->queue_addressing_actions(mode);

	this->_actions.push_back([this, &mode]() { this->read_memory(); });

	this->_actions.push_back([this, &mode]() {
		this->_data_bus(this->_alu.shift_left(this->_data_bus()));
		this->c_flag(this->_alu.carry());
		this->z_flag(this->_data_bus() == 0);
		this->n_flag((this->_data_bus() & 0b10000000) > 0);
	});

	this->_actions.push_back([this, &mode]() { this->write_memory(); });
}

/// @brief Branch if Carry Clear (2-4 cycles)
/// @param mode 0x90 Relative
void Cpu::bcc(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->fetch();

		if (!this->c_flag())
		{
			this->relative();
		}
	});
}

/// @brief Branch if Carry Set (2-4 cycles)
/// @param mode 0xb0 Relative
void Cpu::bcs(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->fetch();

		if (this->c_flag())
		{
			this->relative();
		}
	});
}

/// @brief Branch if Equal (2-4 cycles)
/// @param mode 0xf0 Relative
void Cpu::beq(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->fetch();

		if (this->z_flag())
		{
			this->relative();
		}
	});
}

/// @brief Bit Test (3-4 cycles)
/// @param mode 0x24 Zero Page;
/// 0x2c Absolute;
void Cpu::bit(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->n_flag((this->_data_bus() & 0b10000000) > 0);
		this->v_flag((this->_data_bus() & 0b01000000) > 0);

		this->_data_bus(this->_data_bus() & this->_a);
		this->z_flag(this->_data_bus() == 0);
	});
}

/// @brief Branch if Minus (2-4 cycles)
/// @param mode 0x30 Relative
void Cpu::bmi(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->fetch();

		if (this->n_flag())
		{
			this->relative();
		}
	});
}

/// @brief Branch if Not Equal (2-4 cycles)
/// @param mode 0xd0 Relative
void Cpu::bne(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->fetch();

		if (!this->z_flag())
		{
			this->relative();
		}
	});
}

/// @brief Branch if Positive (2-4 cycles)
/// @param mode 0x10 Relative
void Cpu::bpl(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->fetch();

		if (!this->n_flag())
		{
			this->relative();
		}
	});
}

/// @brief Force Interrupt (7 cycles)
/// @param mode 0x00 Implicit
void Cpu::brk(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->program_counter_high());
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->program_counter_low());
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->b_flag(true);
		this->_data_bus(this->_p);
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->i_flag(true);
		this->_s--;
		this->_address_bus.highByte(0xff);
		this->_address_bus.lowByte(0xfe);
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_input_data_latch = this->_data_bus();
		this->_address_bus.highByte(0xff);
		this->_address_bus.lowByte(0xff);
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->program_counter_low() = this->_input_data_latch;
		this->program_counter_high() = this->_data_bus();
	});
}

/// @brief Branch if Overflow Clear (2-4 cycles)
/// @param mode 0x50 Relative
void Cpu::bvc(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->fetch();

		if (!this->v_flag())
		{
			this->relative();
		}
	});
}

/// @brief Branch if Overflow Set (2-4 cycles)
/// @param mode 0x70 Relative
void Cpu::bvs(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->fetch();

		if (this->v_flag())
		{
			this->relative();
		}
	});
}

/// @brief Clear Carry Flag (2 cycles)
/// @param mode 0x18 Implicit
void Cpu::clc(AddressingMode mode)
{
	this->_actions.push_back([this]() { this->c_flag(false); });
}

/// @brief Clear Decimal Flag (2 cycles)
/// @param mode 0xd8 Implicit
void Cpu::cld(AddressingMode mode)
{
	this->_actions.push_back([this]() { this->d_flag(false); });
}

/// @brief Clear Interrupt Flag (2 cycles)
/// @param mode 0x58 Implicit
void Cpu::cli(AddressingMode mode)
{
	this->_actions.push_back([this]() { this->i_flag(false); });
}

/// @brief Clear Overflow Flag (2 cycles)
/// @param mode 0xb8 Implicit
void Cpu::clv(AddressingMode mode)
{
	this->_actions.push_back([this]() { this->v_flag(false); });
}

/// @brief Compare (2-6 cycles)
/// @param mode 0xc9 Immediate;
/// 0xc5 Zero Page;
/// 0xd5 Zero Page,X;
/// 0xcd Absolute;
/// 0xdd Absolute,X;
/// 0xd9 Absolute,Y;
/// 0xc1 Indirect,X;
/// 0xd1 Indirect,Y;
void Cpu::cmp(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->c_flag(this->_a >= this->_data_bus());
		this->_data_bus(this->_alu.subtract(this->_a, this->_data_bus(), false));
		this->z_flag(this->_data_bus() == 0);
		this->n_flag((this->_data_bus() & 0b10000000) > 0);
	});
}

/// @brief Compare X Register (2-4 cycles)
/// @param mode 0xe0 Immediate;
/// 0xe4 Zero Page;
/// 0xec Absolute;
void Cpu::cpx(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->c_flag(this->_x >= this->_data_bus());
		this->_data_bus(this->_alu.subtract(this->_x, this->_data_bus(), false));
		this->z_flag(this->_data_bus() == 0);
		this->n_flag((this->_data_bus() & 0b10000000) > 0);
	});
}

/// @brief Compare Y Register (2-4 cycles)
/// @param mode 0xc0 Immediate;
/// 0xc4 Zero Page;
/// 0xcc Absolute;
void Cpu::cpy(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->c_flag(this->_y >= this->_data_bus());
		this->_data_bus(this->_alu.subtract(this->_y, this->_data_bus(), false));
		this->z_flag(this->_data_bus() == 0);
		this->n_flag((this->_data_bus() & 0b10000000) > 0);
	});
}

/// @brief Decrement Memory (5-7 cycles)
/// @param mode 0xc6 Zero Page;
/// 0xd6 Zero Page,X;
/// 0xce Absolute;
/// 0xde ABsolute,X;
void Cpu::dec(AddressingMode mode)
{
	this->queue_addressing_actions(mode);

	this->_actions.push_back([this, &mode]() { this->read_memory(); });

	this->_actions.push_back([this, &mode]() {
		this->_data_bus(this->_alu.decrement(this->_data_bus()));
		this->z_flag(this->_data_bus() == 0);
		this->n_flag((this->_data_bus() & 0b10000000) > 0);
	});

	this->_actions.push_back([this, &mode]() { this->write_memory(); });
}

/// @brief Decrement X Register (2 cycles)
/// @param mode 0xca Implicit
void Cpu::dex(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_x--;
		this->z_flag(this->_x == 0);
		this->n_flag((this->_x & 0b10000000) > 0);
	});
}

/// @brief Decrement Y Register (2 cycles)
/// @param mode 0x88 Implicit
void Cpu::dey(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_y--;
		this->z_flag(this->_y == 0);
		this->n_flag((this->_y & 0b10000000) > 0);
	});
}

/// @brief Exclusive OR (2-6 cycles)
/// @param mode 0x49 Immediate;
/// 0x45 Zero Page;
/// 0x55 Zero Page,X;
/// 0x4d Absolute;
/// 0x5d Absolute,X;
/// 0x59 Absolute,Y;
/// 0x41 Indirect,X;
/// 0x51 Indirect,Y;
void Cpu::eor(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->_a ^= this->_data_bus();
		this->z_flag(this->_a == 0);
		this->n_flag((this->_a & 0b10000000) > 0);
	});
}

/// @brief Increment Memory (5-7 cycles)
/// @param mode 0xe6 Zero Page;
/// 0xf6 Zero Page,X;
/// 0xee Absolute;
/// 0xfe ABsolute,X;
void Cpu::inc(AddressingMode mode)
{
	this->queue_addressing_actions(mode);

	this->_actions.push_back([this, &mode]() { this->read_memory(); });

	this->_actions.push_back([this, &mode]() {
		this->_data_bus(this->_alu.increment(this->_data_bus()));
		this->z_flag(this->_data_bus() == 0);
		this->n_flag((this->_data_bus() & 0b10000000) > 0);
	});

	this->_actions.push_back([this, &mode]() { this->write_memory(); });
}

/// @brief Increment X Register (2 cycles)
/// @param mode 0xe8 Implicit
void Cpu::inx(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_x = this->_alu.increment(this->_x);
		this->z_flag(this->_x == 0);
		this->n_flag((this->_x & 0b10000000) > 0);
	});
}

/// @brief Increment Y Register (2 cycles)
/// @param mode 0xc8 Implicit
void Cpu::iny(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_y = this->_alu.increment(this->_y);
		this->z_flag(this->_y == 0);
		this->n_flag((this->_y & 0b10000000) > 0);
	});
}

/// @brief Jump (3-5 cycles)
/// @param mode 0x4c Absolute;
/// 0x6c Indirect;
void Cpu::jmp(AddressingMode mode)
{
	if (mode == AddressingMode::absolute)
	{
		this->_actions.push_back([this]() {
			this->fetch();
			this->_input_data_latch = this->_data_bus();
		});

		this->_actions.push_back([this]() {
			this->fetch();
			this->_address_bus.lowByte(this->_input_data_latch);
			this->_address_bus.highByte(this->_data_bus());
			this->program_counter() = this->address_bus();
		});
	}
	else
	{
		this->indirect();
	}
}

/// @brief Jump to Subroutine (6 cycles)
/// @param mode 0x20 Absolute
void Cpu::jsr(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_input_data_latch = this->_data_bus();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->program_counter_high());
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->program_counter_low());
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
	});

	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.lowByte(this->_input_data_latch);
		this->_address_bus.highByte(this->_data_bus());
		this->program_counter() = this->address_bus();
	});
}

/// @brief Load A Register (2-6 cycles)
/// @param mode 0xa9 Immediate;
/// 0xa5 Zero Page;
/// 0xb5 Zero Page,X;
/// 0xad Absolute;
/// 0xbd Absolute,X;
/// 0xb9 Absolute,Y;
/// 0xa1 Indirect,X;
/// 0xb1 Indirect,Y;
void Cpu::lda(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->_a = this->_data_bus();
		this->z_flag(this->_a == 0);
		this->n_flag((this->_a & 0b10000000) > 0);
	});
}

/// @brief Load X Register (2-5 cycles)
/// @param mode 0xa2 Immediate;
/// 0xa6 Zero Page;
/// 0xb6 Zero Page,Y;
/// 0xae Absolute;
/// 0xbe Absolute,Y;
void Cpu::ldx(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->_x = this->_data_bus();
		this->z_flag(this->_x == 0);
		this->n_flag((this->_x & 0b10000000) > 0);
	});
}

/// @brief Load Y Register (2-5 cycles)
/// @param mode 0xa0 Immediate;
/// 0xa4 Zero Page;
/// 0xb4 Zero Page,X;
/// 0xac Absolute;
/// 0xbc Absolute,X;
void Cpu::ldy(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->_y = this->_data_bus();
		this->z_flag(this->_y == 0);
		this->n_flag((this->_y & 0b10000000) > 0);
	});
}

/// @brief Logical Shift Right (2-7 cycles)
/// @param mode 0x4a Accumulator;
/// 0x46 Zero Page;
/// 0x56 Zero Page,X;
/// 0x4e Absolute;
/// 0x5e Absolute,X;
void Cpu::lsr(AddressingMode mode)
{
	if (mode == AddressingMode::accumulator)
	{
		this->_actions.push_back([this, &mode]() {
			this->_a = this->_alu.shift_right(this->_a);
			this->c_flag(this->_alu.carry());
			this->z_flag(this->_a == 0);
			this->n_flag(0);
		});
		return;
	}

	this->queue_addressing_actions(mode);

	this->_actions.push_back([this, &mode]() { this->read_memory(); });

	this->_actions.push_back([this, &mode]() {
		this->_data_bus(this->_alu.shift_right(this->_data_bus()));
		this->c_flag(this->_alu.carry());
		this->z_flag(this->_data_bus() == 0);
		this->n_flag(0);
	});

	this->_actions.push_back([this, &mode]() { this->write_memory(); });
}

/// @brief No Operation (2 cycles)
/// @param mode 0xea Implicit
void Cpu::nop(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		// no op
	});
}

/// @brief Logical Inclusive OR (2-6 cycles)
/// @param mode 0x09 Immediate;
/// 0x05 Zero Page;
/// 0x15 Zero Page,X;
/// 0x0d Absolute;
/// 0x1d Absolute,X;
/// 0x19 Absolute,Y;
/// 0x01 Indirect,X;
/// 0x11 Indirect,Y;
void Cpu::ora(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->_a |= this->_data_bus();
		this->z_flag(this->_a == 0);
		this->n_flag((this->_a & 0b10000000) > 0);
	});
}

/// @brief Push Accumlulator (3 cycles)
/// @param mode 0x48 Implicit
void Cpu::pha(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->_a);
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
	});
}

/// @brief Push Processor Status (3 cycles)
/// @param mode 0x08 Implicit
void Cpu::php(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->_p);
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
	});
}

/// @brief Pull Accumulator (4 cycles)
/// @param mode 0x68 Implicit
void Cpu::pla(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_s++;
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_a = this->_data_bus();
		this->z_flag(this->_a == 0);
		this->n_flag((this->_a & 0b10000000) > 0);
	});
}

/// @brief Pull Processor Status (4 cycles)
/// @param mode 0x28 Implicit
void Cpu::plp(AddressingMode mode)
{
	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_s++;
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_p = this->_data_bus();
	});
}

/// @brief Rotate Left (2-7 cycles)
/// @param mode 0x2a Accumulator;
/// 0x26 Zero Page;
/// 0x36 Zero Page,X;
/// 0x2e Absolute;
/// 0x3e Absolute,X;
void Cpu::rol(AddressingMode mode)
{
	if (mode == AddressingMode::accumulator)
	{
		this->_actions.push_back([this, &mode]() {
			this->_a = this->_alu.rotate_left(this->_a, this->c_flag());
			this->c_flag(this->_alu.carry());
			this->z_flag(this->_a == 0);
			this->n_flag((this->_a & 0b10000000) > 0);
		});
		return;
	}

	this->queue_addressing_actions(mode);

	this->_actions.push_back([this, &mode]() { this->read_memory(); });

	this->_actions.push_back([this, &mode]() {
		this->_data_bus(this->_alu.rotate_left(this->_data_bus(), this->c_flag()));
		this->c_flag(this->_alu.carry());
		this->z_flag(this->_data_bus() == 0);
		this->n_flag((this->_data_bus() & 0b10000000) > 0);
	});

	this->_actions.push_back([this, &mode]() { this->write_memory(); });
}

/// @brief Rotate Right (2-7 cycles)
/// @param mode 0x6a Accumulator;
/// 0x66 Zero Page;
/// 0x76 Zero Page,X;
/// 0x6e Absolute;
/// 0x7e Absolute,X;
void Cpu::ror(AddressingMode mode)
{
	if (mode == AddressingMode::accumulator)
	{
		this->_actions.push_back([this, &mode]() {
			this->_a = this->_alu.rotate_right(this->_a, this->c_flag());
			this->c_flag(this->_alu.carry());
			this->z_flag(this->_a == 0);
			this->n_flag((this->_a & 0b10000000) > 0);
		});
		return;
	}

	this->queue_addressing_actions(mode);

	this->_actions.push_back([this, &mode]() { this->read_memory(); });

	this->_actions.push_back([this, &mode]() {
		this->_data_bus(this->_alu.rotate_right(this->_data_bus(), this->c_flag()));
		this->c_flag(this->_alu.carry());
		this->z_flag(this->_data_bus() == 0);
		this->n_flag((this->_data_bus() & 0b10000000) > 0);
	});

	this->_actions.push_back([this, &mode]() { this->write_memory(); });
}

/// @brief Return from Interrupt (6 cycles)
/// @param mode 0x40 Implicit
void Cpu::rti(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_s++;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_p = this->_data_bus();
		this->_s++;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_input_data_latch = this->_data_bus();
		this->_s++;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->program_counter_high() = this->_data_bus();
		this->program_counter_low() = this->_input_data_latch;
	});
}

/// @brief Return from Subroutine (6 cycles)
/// @param mode 0x60 Implicit
void Cpu::rts(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->fetch();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_s++;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->_input_data_latch = this->_data_bus();
		this->_s++;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
	});

	this->_actions.push_back([this, &mode]() {
		this->read_memory();
		this->program_counter_high() = this->_data_bus();
		this->program_counter_low() = this->_input_data_latch;
	});

	this->_actions.push_back([this]() { this->fetch(); });
}

/// @brief Subtract with Carry (2-6 cycles)
/// @param mode 0xe9 Immediate;
/// 0xe5 Zero Page;
/// 0xf5 Zero Page,X;
/// 0xed Absolute;
/// 0xfd Absolute,X;
/// 0xf9 Absolute,Y;
/// 0xe1 Indirect,X;
/// 0xf1 Indirect,Y;
void Cpu::sbc(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this, mode]() {
		if (mode == AddressingMode::immediate)
			this->fetch();
		else
			this->read_memory();

		this->_a = this->_alu.subtract(this->_a, this->_data_bus(), !this->c_flag());
		this->c_flag(this->_alu.carry());
		this->v_flag(this->_alu.overflow());
		this->z_flag(this->_a == 0);
		this->n_flag((this->_a & 0b10000000) > 0);
	});
}

/// @brief Set Carry Flag (2 cycles)
/// @param mode 0x38 Implicit
void Cpu::sec(AddressingMode mode)
{
	this->_actions.push_back([this]() { this->c_flag(true); });
}

/// @brief Set Decimal Flag (2 cycles)
/// @param mode 0xf8 implicit
void Cpu::sed(AddressingMode mode)
{
	this->_actions.push_back([this]() { this->d_flag(true); });
}

/// @brief Set Interrupt Disable (2 cycles)
/// @param mode 0x78 Implicit
void Cpu::sei(AddressingMode mode)
{
	this->_actions.push_back([this]() { this->i_flag(true); });
}

/// @brief Store Accumulator (3-6 cycles)
/// @param mode 0x85 Zero Page;
/// 0x95 Zero Page,X;
/// 0x8d Absolute;
/// 0x9d Absolute,X;
/// 0x99 Absolute,Y;
/// 0x81 Indirect,X;
/// 0x91 Indirect,Y
void Cpu::sta(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this]() {
		this->_data_bus(this->_a);
		this->write_memory();
	});
}

/// @brief Store X Register (3-4 cycles)
/// @param mode 0x86 Zero Page;
/// 0x96 Zero Page,X; 0x8e Absolute;
void Cpu::stx(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this]() {
		this->_data_bus(this->_x);
		this->write_memory();
	});
}

/// @brief Store Y Register (3-4 cycles)
/// @param mode 0x84 Zero Page;
/// 0x94 Zero Page,X; 0x8c Absolute;
void Cpu::sty(AddressingMode mode)
{
	queue_addressing_actions(mode);

	this->_actions.push_back([this]() {
		this->_data_bus(this->_y);
		this->write_memory();
	});
}

/// @brief Transfer Accumulator to X (2 cycles)
/// @param mode 0xaa Implicit
void Cpu::tax(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_x = this->_a;
		this->z_flag(this->_x == 0);
		this->n_flag((this->_x & 0b10000000) > 0);
	});
}

/// @brief Transfer Accumulator to Y (2 cycles)
/// @param mode 0xa8 Implicit
void Cpu::tay(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_y = this->_a;
		this->z_flag(this->_y == 0);
		this->n_flag((this->_y & 0b10000000) > 0);
	});
}

/// @brief Transfer Stack Pointer to X (2 cycles)
/// @param mode 0xba Implicit
void Cpu::tsx(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_x = this->_s;
		this->z_flag(this->_x == 0);
		this->n_flag((this->_x & 0b10000000) > 0);
	});
}

/// @brief Transfer X to Accumulator (2 cycles)
/// @param mode 0x8a Implicit
void Cpu::txa(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_a = this->_x;
		this->z_flag(this->_a == 0);
		this->n_flag((this->_a & 0b10000000) > 0);
	});
}

/// @brief Transfer X to Stack Pointer (2 cycles)
/// @param mode 0x9a Implicit
void Cpu::txs(AddressingMode mode)
{
	this->_actions.push_back([this]() { this->_s = this->_x; });
}

/// @brief Transfer Y to Accumulator (2 cycles)
/// @param mode 0x98 Implicit
void Cpu::tya(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_a = this->_y;
		this->z_flag(this->_a == 0);
		this->n_flag((this->_a & 0b10000000) > 0);
	});
}

void Cpu::nmi(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_address_bus(this->program_counter());
		this->read_memory();
		this->_instruction = 0x00;
		this->_address_bus(this->program_counter());
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->program_counter_high());
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->program_counter_low());
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->b_flag(false);
		this->_data_bus(this->_p);
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0xff);
		this->_address_bus.lowByte(0xfa);
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_input_data_latch = this->_data_bus();
		this->_address_bus.highByte(0xff);
		this->_address_bus.lowByte(0xfb);
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->program_counter_low() = this->_input_data_latch;
		this->program_counter_high() = this->_data_bus();
	});
}

void Cpu::irq(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_address_bus(this->program_counter());
		this->read_memory();
		this->_instruction = 0x00;
		this->_address_bus(this->program_counter());
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->program_counter_high());
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->program_counter_low());
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->b_flag(false);
		this->_data_bus(this->_p);
	});

	this->_actions.push_back([this, &mode]() {
		this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0xff);
		this->_address_bus.lowByte(0xfe);
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_input_data_latch = this->_data_bus();
		this->_address_bus.highByte(0xff);
		this->_address_bus.lowByte(0xff);
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->program_counter_low() = this->_input_data_latch;
		this->program_counter_high() = this->_data_bus();
	});
}

void Cpu::res(AddressingMode mode)
{
	this->_actions.push_back([this]() {
		this->_address_bus(this->program_counter());
		this->read_memory();
		this->_instruction = 0x00;
		this->_address_bus(this->program_counter());
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->program_counter_high());
	});

	this->_actions.push_back([this, &mode]() {
		// this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->_data_bus(this->program_counter_low());
	});

	this->_actions.push_back([this, &mode]() {
		// this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0x01);
		this->_address_bus.lowByte(this->_s);
		this->b_flag(false);
		this->_data_bus(this->_p);
	});

	this->_actions.push_back([this, &mode]() {
		// this->write_memory();
		this->_s--;
		this->_address_bus.highByte(0xff);
		this->_address_bus.lowByte(0xfc);
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->_input_data_latch = this->_data_bus();
		this->_address_bus.highByte(0xff);
		this->_address_bus.lowByte(0xfd);
	});

	this->_actions.push_back([this]() {
		this->read_memory();
		this->program_counter_low() = this->_input_data_latch;
		this->program_counter_high() = this->_data_bus();
	});
}
