#include <catch2/catch_test_macros.hpp>
#include <cpu/cpu.hpp>

#include "../mockMemory.hpp"

SCENARIO("Can perform tax operation", "[cpu][transfer_ops]")
{
	MockMemory mem;
	mem._data[0] = 0xaa;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("accumulator has value of 7")
	{
		cpu.accumulator(7);

		WHEN("perform tax operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("x register has value of 7")
			{
				REQUIRE(cpu.x_register() == 7);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("accumulator has value of 0")
	{
		cpu.accumulator(0);

		WHEN("perform tax operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("x register has value of 0")
			{
				REQUIRE(cpu.x_register() == 0);

				AND_THEN("zero flag is set")
				{
					REQUIRE(cpu.z_flag() == true);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("accumulator has value of 140")
	{
		cpu.accumulator(140);

		WHEN("perform tax operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("x register has value of 140")
			{
				REQUIRE(cpu.x_register() == 140);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is set")
					{
						REQUIRE(cpu.n_flag() == true);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform tay operation", "[cpu][transfer_ops]")
{
	MockMemory mem;
	mem._data[0] = 0xa8;
	Cpu cpu(&mem);

	GIVEN("accumulator has value of 77")
	{
		cpu.program_counter(0x0000);
		cpu.accumulator(77);

		WHEN("perform tay operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("y register has value of 77")
			{
				REQUIRE(cpu.y_register() == 77);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("accumulator has value of 0")
	{
		cpu.program_counter(0x0000);
		cpu.accumulator(0);

		WHEN("perform tay operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("y register has value of 0")
			{
				REQUIRE(cpu.y_register() == 0);

				AND_THEN("zero flag is set")
				{
					REQUIRE(cpu.z_flag() == true);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("accumulator has value of 255")
	{
		cpu.program_counter(0x0000);
		cpu.accumulator(255);

		WHEN("perform tay operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("y register has value of 255")
			{
				REQUIRE(cpu.y_register() == 255);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is set")
					{
						REQUIRE(cpu.n_flag() == true);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform tsx operation", "[cpu][transfer_ops]")
{
	MockMemory mem;
	mem._data[0] = 0xba;
	Cpu cpu(&mem);

	GIVEN("stack pointer has value of 13")
	{
		cpu.program_counter(0x0000);
		cpu.stack_register(13);

		WHEN("perform tsx operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("x register has value of 13")
			{
				REQUIRE(cpu.x_register() == 13);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("stack pointer has value of 0")
	{
		cpu.program_counter(0x0000);
		cpu.stack_register(0);

		WHEN("perform tsx operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("x register has value of 0")
			{
				REQUIRE(cpu.x_register() == 0);

				AND_THEN("zero flag is set")
				{
					REQUIRE(cpu.z_flag() == true);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("stack pointer has value of 157")
	{
		cpu.program_counter(0x0000);
		cpu.stack_register(157);

		WHEN("perform tsx operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("x register has value of 157")
			{
				REQUIRE(cpu.x_register() == 157);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is set")
					{
						REQUIRE(cpu.n_flag() == true);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform txa operation", "[cpu][transfer_ops]")
{
	MockMemory mem;
	mem._data[0] = 0x8a;
	Cpu cpu(&mem);

	GIVEN("x register has value of 2")
	{
		cpu.program_counter(0x0000);
		cpu.x_register(2);

		WHEN("perform txa operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("accumulator has value of 2")
			{
				REQUIRE(cpu.accumulator() == 2);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("x register has value of 0")
	{
		cpu.program_counter(0x0000);
		cpu.x_register(0);

		WHEN("perform txa operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("accumulator has value of 0")
			{
				REQUIRE(cpu.accumulator() == 0);

				AND_THEN("zero flag is set")
				{
					REQUIRE(cpu.z_flag() == true);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("x register has value of 200")
	{
		cpu.program_counter(0x0000);
		cpu.x_register(200);

		WHEN("perform txa operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("accumulator has value of 200")
			{
				REQUIRE(cpu.accumulator() == 200);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is set")
					{
						REQUIRE(cpu.n_flag() == true);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform txs operation", "[cpu][transfer_ops]")
{
	MockMemory mem;
	mem._data[0] = 0x9a;
	Cpu cpu(&mem);

	GIVEN("x register has value of 21")
	{
		cpu.program_counter(0x0000);
		cpu.x_register(21);

		WHEN("perform txs operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("stack pointer has value of 21")
			{
				REQUIRE(cpu.stack_register() == 21);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("x register has value of 0")
	{
		cpu.program_counter(0x0000);
		cpu.x_register(0);

		WHEN("perform txs operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("stack pointer has value of 0")
			{
				REQUIRE(cpu.stack_register() == 0);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("x register has value of 211")
	{
		cpu.program_counter(0x0000);
		cpu.x_register(211);

		WHEN("perform txs operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("stack pointer has value of 211")
			{
				REQUIRE(cpu.stack_register() == 211);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform tya operation", "[cpu][transfer_ops]")
{
	MockMemory mem;
	mem._data[0] = 0x98;
	Cpu cpu(&mem);

	GIVEN("y register has value of 87")
	{
		cpu.program_counter(0x0000);
		cpu.y_register(87);

		WHEN("perform tya operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("accumulator has value of 87")
			{
				REQUIRE(cpu.accumulator() == 87);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("y register has value of 0")
	{
		cpu.program_counter(0x0000);
		cpu.y_register(0);

		WHEN("perform tya operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("accumulator has value of 0")
			{
				REQUIRE(cpu.accumulator() == 0);

				AND_THEN("zero flag is set")
				{
					REQUIRE(cpu.z_flag() == true);

					AND_THEN("negative flag is not set")
					{
						REQUIRE(cpu.n_flag() == false);
					}
				}
			}
		}
	}

	GIVEN("y register has value of 172")
	{
		cpu.program_counter(0x0000);
		cpu.y_register(172);

		WHEN("perform tya operation (2 cycles)")
		{
			cpu.cycle();
			cpu.cycle();

			THEN("accumulator has value of 172")
			{
				REQUIRE(cpu.accumulator() == 172);

				AND_THEN("zero flag is not set")
				{
					REQUIRE(cpu.z_flag() == false);

					AND_THEN("negative flag is set")
					{
						REQUIRE(cpu.n_flag() == true);
					}
				}
			}
		}
	}
}
