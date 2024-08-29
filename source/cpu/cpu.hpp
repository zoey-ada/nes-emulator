#pragma once

#include <array>
#include <deque>
#include <functional>
#include <map>

#include <base/iCpu.hpp>
#include <base/register.hpp>

#include "addressing_modes.hpp"
#include "alu.hpp"

using Register = uint8_t;

class Cpu;
class IMemory;

struct Operation
{
	void (Cpu::*operation)(AddressingMode);
	AddressingMode addressing_mode;
};

using OperationMapping = std::pair<uint8_t, Operation>;
using OperationMap = std::map<uint8_t, Operation>;

class Cpu;
using InterruptDelegate = void (Cpu::*)();
using AddressingFuncDelegate = void (Cpu::*)();

using AddressingModeMapping = std::pair<AddressingMode, AddressingFuncDelegate>;
using AddressingModeMap = std::map<AddressingMode, AddressingFuncDelegate>;

using Action = std::function<void()>;

class Cpu: public ICpu
{
public:
	explicit Cpu(IMemory* memory);
	virtual ~Cpu();

	void cycle() override;
	void cycle(const uint64_t number_of_cycles) override;

	uint16_t address_bus() const override { return this->_address_bus(); }
	uint8_t data_bus() const override { return this->_data_bus(); }
	void data_bus(const uint8_t data) override { this->_data_bus(data); }

	inline bool audio_out_1() const override { return this->_audio_out_1; }
	inline bool audio_out_2() const override { return this->_audio_out_2; }
	inline bool read_write() const override { return this->_read_write; }

	void irq() override;
	void nmi() override;
	void reset() override;

	inline void suspend() { this->_is_suspended = true; }
	inline void activate() { this->_is_suspended = false; }

	//--------------------------------------------------------------------------
	// registers
	//--------------------------------------------------------------------------
	uint16_t& program_counter() { return (uint16_t&)this->_program_counter[0]; }
	void program_counter(const uint16_t data) { (uint16_t&)this->_program_counter[0] = data; }
	// bytes are reversed so that reading the 16 bit values is correct
	uint8_t& program_counter_high() { return this->_program_counter[1]; }
	uint8_t& program_counter_low() { return this->_program_counter[0]; }

	inline uint8_t accumulator() const { return this->_a; }
	inline void accumulator(const uint8_t data) { this->_a = data; }

	inline uint8_t x_register() const { return this->_x; }
	inline void x_register(const uint8_t data) { this->_x = data; }

	inline uint8_t y_register() const { return this->_y; }
	inline void y_register(const uint8_t data) { this->_y = data; }

	inline uint8_t stack_register() const { return this->_s; }
	inline void stack_register(const uint8_t data) { this->_s = data; }

	inline uint8_t status_register() const { return this->_p; }
	inline void status_register(const uint8_t data) { this->_p = data; }

	inline bool n_flag() const { return (this->_p & 0b10000000) > 0; }
	inline void n_flag(const bool state)
	{
		state ? this->_p |= 0b10000000 : this->_p &= 0b01111111;
	};

	inline bool v_flag() const { return (this->_p & 0b01000000) > 0; }
	inline void v_flag(const bool state)
	{
		state ? this->_p |= 0b01000000 : this->_p &= 0b10111111;
	};

	inline bool b_flag() const { return (this->_p & 0b00010000) > 0; }
	inline void b_flag(const bool state)
	{
		state ? this->_p |= 0b00010000 : this->_p &= 0b11101111;
	};

	inline bool d_flag() const { return (this->_p & 0b00001000) > 0; }
	inline void d_flag(const bool state)
	{
		state ? this->_p |= 0b00001000 : this->_p &= 0b11110111;
	};

	inline bool i_flag() const { return (this->_p & 0b00000100) > 0; }
	inline void i_flag(const bool state)
	{
		state ? this->_p |= 0b00000100 : this->_p &= 0b11111011;
	};

	inline bool z_flag() const { return (this->_p & 0b00000010) > 0; }
	inline void z_flag(const bool state)
	{
		state ? this->_p |= 0b00000010 : this->_p &= 0b11111101;
	};

	inline bool c_flag() const { return (this->_p & 0b00000001) > 0; }
	inline void c_flag(const bool state)
	{
		state ? this->_p |= 0b00000001 : this->_p &= 0b11111110;
	};

protected:
	IMemory* _memory;
	std::deque<Action> _actions;
	bool _executing_interrupt {false};
	bool _is_interrupt_queued {false};
	InterruptDelegate _queued_interrupt_func {nullptr};

	inline Register instruction_register() { return this->_instruction; }
	inline Register input_data_latch() { return this->_input_data_latch; }

	virtual void queue_next_instruction();

	virtual void nmi_impl();
	virtual void irq_impl();

private:
	// registers
	Register _a {0x00};
	Register _x {0x00};
	Register _y {0x00};
	Register _s {0xfd};
	Register _p {0x34};
	Register _instruction {0x00};
	Register _input_data_latch {0x00};
	std::array<uint8_t, 2> _program_counter = {0x00, 0x00};

	// pins
	Register_16bit _address_bus;
	Register_8bit _data_bus;
	bool _audio_out_1 {false};
	bool _audio_out_2 {false};
	bool _read_write {false};  // false -> write; true -> read

	Alu _alu;
	OperationMap _operations;
	AddressingModeMap _addressing_modes;

	bool _is_suspended {false};

	void clear_registers();

	// read from the address the PC (program counter) is pointing to
	void fetch();
	void read_memory();
	void write_memory();

	void populate_operations();
	void populate_addressing_modes();
	void queue_addressing_actions(AddressingMode mode);

	void no_address();
	void relative();

	void zero_page();
	void zero_page_x();
	void zero_page_y();

	void absolute();
	void absolute_x_read();
	void absolute_x_write();
	void absolute_y_read();
	void absolute_y_write();

	void indirect();
	void indirect_x();
	void indirect_y_read();
	void indirect_y_write();

	void adc(AddressingMode mode);
	void and_(AddressingMode mode);
	void asl(AddressingMode mode);
	void bcc(AddressingMode mode);
	void bcs(AddressingMode mode);
	void beq(AddressingMode mode);
	void bit(AddressingMode mode);
	void bmi(AddressingMode mode);
	void bne(AddressingMode mode);
	void bpl(AddressingMode mode);
	void brk(AddressingMode mode);
	void bvc(AddressingMode mode);
	void bvs(AddressingMode mode);
	void clc(AddressingMode mode);
	void cld(AddressingMode mode);
	void cli(AddressingMode mode);
	void clv(AddressingMode mode);

	void cmp(AddressingMode mode);
	void cpx(AddressingMode mode);
	void cpy(AddressingMode mode);

	void dec(AddressingMode mode);
	void dex(AddressingMode mode);
	void dey(AddressingMode mode);

	void eor(AddressingMode mode);

	void inc(AddressingMode mode);
	void inx(AddressingMode mode);
	void iny(AddressingMode mode);

	void jmp(AddressingMode mode);
	void jsr(AddressingMode mode);

	void lda(AddressingMode mode);
	void ldx(AddressingMode mode);
	void ldy(AddressingMode mode);

	void lsr(AddressingMode mode);

	void nop(AddressingMode mode);

	void ora(AddressingMode mode);
	void pha(AddressingMode mode);
	void php(AddressingMode mode);
	void pla(AddressingMode mode);
	void plp(AddressingMode mode);
	void rol(AddressingMode mode);
	void ror(AddressingMode mode);
	void rti(AddressingMode mode);
	void rts(AddressingMode mode);
	void sbc(AddressingMode mode);

	void sec(AddressingMode mode);
	void sed(AddressingMode mode);
	void sei(AddressingMode mode);

	void sta(AddressingMode mode);
	void stx(AddressingMode mode);
	void sty(AddressingMode mode);

	void tax(AddressingMode mode);
	void tay(AddressingMode mode);
	void tsx(AddressingMode mode);
	void txa(AddressingMode mode);
	void txs(AddressingMode mode);
	void tya(AddressingMode mode);

	void res();

	// unofficial operations
	void jam(AddressingMode mode);
};
