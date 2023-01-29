#include <catch2/catch_test_macros.hpp>
#include <cpu/cpu.hpp>

#include "../mockMemory.hpp"

SCENARIO("Can perform sec operation", "[cpu][status_register_ops]")
{
	MockMemory mem;
	mem._data[0] = 0x38;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("status register is 0x00")
	{
		cpu.status_register(0x00);

		WHEN("perform sec operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("carry flag is set")
			{
				REQUIRE(cpu.c_flag() == true);
			}
		}
	}
}

SCENARIO("Can perform sed operation", "[cpu][status_register_ops]")
{
	MockMemory mem;
	mem._data[0] = 0xf8;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("status register is 0x00")
	{
		cpu.status_register(0x00);

		WHEN("perform sec operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("decimal flag is set")
			{
				REQUIRE(cpu.d_flag() == true);
			}
		}
	}
}

SCENARIO("Can perform sei operation", "[cpu][status_register_ops]")
{
	MockMemory mem;
	mem._data[0] = 0x78;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("status register is 0x00")
	{
		cpu.status_register(0x00);

		WHEN("perform sei operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("interrupt disable flag is set")
			{
				REQUIRE(cpu.i_flag() == true);
			}
		}
	}
}

SCENARIO("Can perform clc operation", "[cpu][status_register_ops]")
{
	MockMemory mem;
	mem._data[0] = 0x18;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("status register is 0xff")
	{
		cpu.status_register(0x00);

		WHEN("perform clc operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("carry flag is cleared")
			{
				REQUIRE(cpu.c_flag() == false);
			}
		}
	}
}

SCENARIO("Can perform cld operation", "[cpu][status_register_ops]")
{
	MockMemory mem;
	mem._data[0] = 0xd8;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("status register is 0xff")
	{
		cpu.status_register(0x00);

		WHEN("perform clc operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("decimal flag is cleared")
			{
				REQUIRE(cpu.d_flag() == false);
			}
		}
	}
}

SCENARIO("Can perform cli operation", "[cpu][status_register_ops]")
{
	MockMemory mem;
	mem._data[0] = 0x58;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("status register is 0xff")
	{
		cpu.status_register(0x00);

		WHEN("perform cli operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("interrupt disable flag is cleared")
			{
				REQUIRE(cpu.i_flag() == false);
			}
		}
	}
}

SCENARIO("Can perform clv operation", "[cpu][status_register_ops]")
{
	MockMemory mem;
	mem._data[0] = 0xb8;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("status register is 0xff")
	{
		cpu.status_register(0x00);

		WHEN("perform clc operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("overflow flag is cleared")
			{
				REQUIRE(cpu.v_flag() == false);
			}
		}
	}
}
