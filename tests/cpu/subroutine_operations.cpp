// brk, rti
#include <catch2/catch_test_macros.hpp>
#include <cpu/cpu.hpp>
#include <memory/slowMemory.hpp>

SCENARIO("Can perform jsr operation", "[cpu][subroutine_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is jsr [absolute]")
	{
		mem._data[0] = 0x20;

		AND_GIVEN("address value is 0x1215 and stack register value is 0xf2")
		{
			mem._data[2] = 0x12;
			mem._data[1] = 0x15;
			cpu.stack_register(0xf2);

			WHEN("operation is performed (6 cycles)")
			{
				cpu.cycle(6);

				THEN(
					"stack register has a value of 0xf0, "
					"program counter has a value of 0x1215, and "
					"previous program counter value (0x0002) is on the stack")
				{
					REQUIRE(cpu.stack_register() == 0xf0);
					REQUIRE(cpu.program_counter() == 0x1215);
					REQUIRE(mem._data[0x01f2] == 0x00);
					REQUIRE(mem._data[0x01f1] == 0x02);
				}
			}
		}
	}
}

SCENARIO("Can perform rts operation", "[cpu][subroutine_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is rts [implicit]")
	{
		mem._data[0x1215] = 0x60;

		AND_GIVEN(
			"program counter value is 0x1215, "
			"status register value is 0x00, "
			"stack register value is 0xf0, and "
			"0x02, 0x00 is stored on the stack")
		{
			cpu.program_counter(0x1215);
			cpu.status_register(0x00);
			cpu.stack_register(0xf0);
			mem._data[0x01f1] = 0x02;
			mem._data[0x01f2] = 0x00;

			WHEN("operation is performed (6 cycles)")
			{
				cpu.cycle(6);

				THEN(
					"stack register has a value of 0xf2, "
					"status register value is 0x00, and "
					"program counter has a value of 0x0003")
				{
					REQUIRE(cpu.stack_register() == 0xf2);
					REQUIRE(cpu.status_register() == 0x00);
					REQUIRE(cpu.program_counter() == 0x0003);
				}
			}
		}
	}
}

SCENARIO("Can perform brk operation", "[cpu][subroutine_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is brk [implicit]")
	{
		mem._data[0] = 0x00;

		AND_GIVEN(
			"address value 0xffff is 0x12, "
			"address value 0xfffe is 0x15, "
			"status register is 0b10000011,and "
			"stack register value is 0xf3")
		{
			mem._data[0xffff] = 0x12;
			mem._data[0xfffe] = 0x15;
			cpu.status_register(0b10000011);
			cpu.stack_register(0xf3);

			WHEN("operation is performed (7 cycles)")
			{
				cpu.cycle(7);

				THEN(
					"stack register has a value of 0xf0, "
					"program counter has a value of 0x1215, "
					"b flag is set, "
					"previous program counter value (0x0002) is on the stack, and "
					"previous status register value (0b10000011) is on the stack")
				{
					REQUIRE(cpu.stack_register() == 0xf0);
					REQUIRE(cpu.program_counter() == 0x1215);
					REQUIRE(cpu.i_flag());
					REQUIRE(mem._data[0x01f3] == 0x00);
					REQUIRE(mem._data[0x01f2] == 0x02);
					REQUIRE(mem._data[0x01f1] == 0b10000011);
				}
			}
		}
	}
}

SCENARIO("Can perform rti operation", "[cpu][subroutine_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is rti [implicit]")
	{
		mem._data[0x1215] = 0x40;

		AND_GIVEN(
			"program counter value is 0x1215, "
			"status register value is 0x00, "
			"stack register value is 0xf0, and "
			"0b10000011, 0x02, 0x00 is stored on the stack")
		{
			cpu.program_counter(0x1215);
			cpu.status_register(0x00);
			cpu.stack_register(0xf0);
			mem._data[0x01f1] = 0b10000011;
			mem._data[0x01f2] = 0x02;
			mem._data[0x01f3] = 0x00;

			WHEN("operation is performed (6 cycles)")
			{
				cpu.cycle(6);

				THEN(
					"stack register has a value of 0xf3, "
					"status register value is 0x0b10000011, and "
					"program counter has a value of 0x0002")
				{
					REQUIRE(cpu.stack_register() == 0xf3);
					REQUIRE(cpu.status_register() == 0b10000011);
					REQUIRE(cpu.program_counter() == 0x0002);
				}
			}
		}
	}
}
