#include <catch2/catch_test_macros.hpp>
#include <cpu/cpu.hpp>
#include <memory/slowMemory.hpp>

SCENARIO("Can perform sta operation", "[cpu][store_ops]")
{
	GIVEN("accumulator has value of 7 and address byte is 0x13")
	{
		SlowMemory mem;
		mem._data[0] = 0x85;
		mem._data[1] = 0x13;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.accumulator(7);

		WHEN("perform sta operation [zero page] (3 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x13 has a value of 7")
			{
				REQUIRE(mem._data[0x13] == 7);
			}
		}
	}

	GIVEN("accumulator has value of 7, x has value of 0x0a, and address byte is 0x13")
	{
		SlowMemory mem;
		mem._data[0] = 0x95;
		mem._data[1] = 0x13;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.accumulator(7);
		cpu.x_register(0x0a);

		WHEN("perform sta operation [zero page, x] (4 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x1d has a value of 7")
			{
				REQUIRE(mem._data[0x1d] == 7);
			}
		}
	}

	GIVEN("accumulator has value of 7 and address bytes are 0x1004")
	{
		SlowMemory mem;
		mem._data[0] = 0x8d;
		// address needs to be written in reverse order
		mem._data[2] = 0x10;
		mem._data[1] = 0x04;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.accumulator(7);

		WHEN("perform sta operation [absolute] (4 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x1004 has a value of 7")
			{
				REQUIRE(mem._data[0x1004] == 7);
			}
		}
	}

	GIVEN("accumulator has value of 7, x has value of 0x02, and address bytes are 0x1004")
	{
		SlowMemory mem;
		mem._data[0] = 0x9d;
		// address needs to be written in reverse order
		mem._data[2] = 0x10;
		mem._data[1] = 0x04;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.accumulator(7);
		cpu.x_register(0x02);

		WHEN("perform sta operation [absolute, x] (5 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x1006 has a value of 7")
			{
				REQUIRE(mem._data[0x1006] == 7);
			}
		}
	}

	GIVEN("accumulator has value of 7, y has value of 0x0a, and address bytes are 0x1004")
	{
		SlowMemory mem;
		mem._data[0] = 0x99;
		// address needs to be written in reverse order
		mem._data[2] = 0x10;
		mem._data[1] = 0x04;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.accumulator(7);
		cpu.y_register(0x0a);

		WHEN("perform sta operation [absolute, y] (5 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x1006 has a value of 7")
			{
				REQUIRE(mem._data[0x100e] == 7);
			}
		}
	}

	GIVEN(
		"accumulator has value of 7, x has value of 0x04, address byte is 0x04, and indirect "
		"address is 0x0115")
	{
		SlowMemory mem;
		mem._data[0] = 0x81;
		mem._data[1] = 0x04;
		mem._data[0x08] = 0x15;
		mem._data[0x09] = 0x01;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.accumulator(7);
		cpu.x_register(0x04);

		WHEN("perform sta operation [indirect x] (6 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x0115 has a value of 7")
			{
				REQUIRE(mem._data[0x0115] == 7);
			}
		}
	}

	GIVEN(
		"accumulator has value of 7, y has value of 0x04, address byte is 0x08, and indirect "
		"address is 0x0115")
	{
		SlowMemory mem;
		mem._data[0] = 0x91;
		mem._data[1] = 0x08;
		mem._data[0x08] = 0x15;
		mem._data[0x09] = 0x01;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.accumulator(7);
		cpu.y_register(0x04);

		WHEN("perform sta operation [indirect y] (6 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x0115 has a value of 7")
			{
				REQUIRE(mem._data[0x0119] == 7);
			}
		}
	}
}

SCENARIO("Can perform stx operation", "[cpu][store_ops]")
{
	GIVEN("x has value of 7 and address byte is 0x13")
	{
		SlowMemory mem;
		mem._data[0] = 0x86;
		mem._data[1] = 0x13;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.x_register(7);

		WHEN("perform stx operation [zero page] (3 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x13 has a value of 7")
			{
				REQUIRE(mem._data[0x13] == 7);
			}
		}
	}

	GIVEN("x has value of 7, y has value of 0x0a, and address byte is 0x13")
	{
		SlowMemory mem;
		mem._data[0] = 0x96;
		mem._data[1] = 0x13;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.x_register(7);
		cpu.y_register(0x0a);

		WHEN("perform stx operation [zero page, y] (4 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x1d has a value of 7")
			{
				REQUIRE(mem._data[0x1d] == 7);
			}
		}
	}

	GIVEN("y has value of 7 and address bytes are 0x1004")
	{
		SlowMemory mem;
		mem._data[0] = 0x8e;
		// address needs to be written in reverse order
		mem._data[2] = 0x10;
		mem._data[1] = 0x04;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.x_register(7);

		WHEN("perform stx operation [absolute] (4 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x1004 has a value of 7")
			{
				REQUIRE(mem._data[0x1004] == 7);
			}
		}
	}
}

SCENARIO("Can perform sty operation", "[cpu][store_ops]")
{
	GIVEN("y has value of 7 and address byte is 0x13")
	{
		SlowMemory mem;
		mem._data[0] = 0x84;
		mem._data[1] = 0x13;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.y_register(7);

		WHEN("perform sty operation [zero page] (3 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x13 has a value of 7")
			{
				REQUIRE(mem._data[0x13] == 7);
			}
		}
	}

	GIVEN("y has value of 7, x has value of 0x0a, and address byte is 0x13")
	{
		SlowMemory mem;
		mem._data[0] = 0x94;
		mem._data[1] = 0x13;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.y_register(7);
		cpu.x_register(0x0a);

		WHEN("perform sty operation [zero page, x] (4 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x1d has a value of 7")
			{
				REQUIRE(mem._data[0x1d] == 7);
			}
		}
	}

	GIVEN("y has value of 7 and address bytes are 0x1004")
	{
		SlowMemory mem;
		mem._data[0] = 0x8c;
		// address needs to be written in reverse order
		mem._data[2] = 0x10;
		mem._data[1] = 0x04;

		Cpu cpu(&mem);
		cpu.program_counter(0x0000);
		cpu.y_register(7);

		WHEN("perform sty operation [absolute] (4 cycles)")
		{
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();

			THEN("address 0x1004 has a value of 7")
			{
				REQUIRE(mem._data[0x1004] == 7);
			}
		}
	}
}
