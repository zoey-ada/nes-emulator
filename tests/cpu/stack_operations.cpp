// pha, php, pla, plp
#include <catch2/catch_test_macros.hpp>
#include <cpu/cpu.hpp>

#include "../mockMemory.hpp"

SCENARIO("Can perform pha operation", "[cpu][stack_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is pha [implicit]")
	{
		mem._data[0] = 0x48;

		AND_GIVEN("accumulator value is 0xf3 and stack register value is 0x13")
		{
			cpu.accumulator(0xf3);
			cpu.stack_register(0x13);

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN(
					"address 0x0113 has a value of 0xf3 and "
					"stack register has a value of 0x12")
				{
					REQUIRE(mem._data[0x0113] == 0xf3);
					REQUIRE(cpu.stack_register() == 0x12);
				}
			}
		}
	}
}

SCENARIO("Can perform php operation", "[cpu][stack_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is php [implicit]")
	{
		mem._data[0] = 0x08;

		AND_GIVEN("accumulator value is 0xc8 and stack register value is 0xf3")
		{
			cpu.status_register(0xc8);
			cpu.stack_register(0xf3);

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN(
					"address 0x01f3 has a value of 0xc8 and "
					"stack register has a value of 0xf2")
				{
					REQUIRE(mem._data[0x01f3] == 0xc8);
					REQUIRE(cpu.stack_register() == 0xf2);
				}
			}
		}
	}
}

SCENARIO("Can perform pla operation", "[cpu][stack_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is pla [implicit]")
	{
		mem._data[0] = 0x68;

		AND_GIVEN("stack register value is 0x12")
		{
			cpu.stack_register(0x12);

			AND_GIVEN("address 0x0113 value is 0x00")
			{
				mem._data[0x0113] = 0x00;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0x00, "
						"stack register has a value of 0x13, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x00);
						REQUIRE(cpu.stack_register() == 0x13);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0113 value is 0x73")
			{
				mem._data[0x0113] = 0x73;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0x73, "
						"stack register has a value of 0x13, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x73);
						REQUIRE(cpu.stack_register() == 0x13);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0113 value is 0xf3")
			{
				mem._data[0x0113] = 0xf3;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0xf3, "
						"stack register has a value of 0x13, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0xf3);
						REQUIRE(cpu.stack_register() == 0x13);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}
}

SCENARIO("Can perform plp operation", "[cpu][stack_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is plp [implicit]")
	{
		mem._data[0] = 0x28;

		AND_GIVEN("address 0x01f3 value is 0xc8 and stack register value is 0xf2")
		{
			mem._data[0x01f3] = 0xc8;
			cpu.stack_register(0xf2);

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN(
					"status register has a value of 0xc8 and "
					"stack register has a value of 0xf3")
				{
					REQUIRE(cpu.status_register() == 0xc8);
					REQUIRE(cpu.stack_register() == 0xf3);
				}
			}
		}
	}
}
