#include <catch2/catch_test_macros.hpp>
#include <cpu/cpu.hpp>

#include "../mockMemory.hpp"

SCENARIO("Can perform asl operation", "[cpu][shift_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is asl [accumulator]")
	{
		mem._data[0] = 0x0a;

		AND_GIVEN("accumulator value is 0b00000000")
		{
			cpu.accumulator(0b00000000);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b00000000, "
					"c flag is not set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b00000000);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b10000000")
		{
			cpu.accumulator(0b10000000);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0x00, "
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x00000000);
					REQUIRE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b11100000")
		{
			cpu.accumulator(0b11100000);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b11000000, "
					"c flag is set, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0b11000000);
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b01100000")
		{
			cpu.accumulator(0b01100000);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b11000000, "
					"c flag is not set, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0b11000000);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b00100101")
		{
			cpu.accumulator(0b00100101);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b01001010, "
					"c flag is not set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b01001010);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b10100101")
		{
			cpu.accumulator(0b10100101);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b01001010, "
					"c flag is set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b01001010);
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is asl [zero page]")
	{
		mem._data[0] = 0x06;

		AND_GIVEN("address byte is 0x12")
		{
			mem._data[1] = 0x12;

			AND_GIVEN("address 0x0012 value is 0b00000000")
			{
				mem._data[0x0012] = 0b00000000;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b10000000")
			{
				mem._data[0x0012] = 0b10000000;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b11100000")
			{
				mem._data[0x0012] = 0b11100000;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b11000000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0012] == 0b11000000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b01100000")
			{
				mem._data[0x0012] = 0b01100000;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b11000000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0012] == 0b11000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b00100101")
			{
				mem._data[0x0012] = 0b00100101;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b01001010, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b01001010);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b10100101")
			{
				mem._data[0x0012] = 0b10100101;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b01001010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b01001010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is asl [zero page, x]")
	{
		mem._data[0] = 0x16;

		AND_GIVEN("address byte is 0x12 and x register value is 0x04")
		{
			mem._data[1] = 0x12;
			cpu.x_register(0x04);

			AND_GIVEN("address 0x0016 value is 0b00000000")
			{
				mem._data[0x0016] = 0b00000000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b10000000")
			{
				mem._data[0x0016] = 0b10000000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b11100000")
			{
				mem._data[0x0016] = 0b11100000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b11000000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0016] == 0b11000000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b01100000")
			{
				mem._data[0x0016] = 0b01100000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b11000000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0016] == 0b11000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b00100101")
			{
				mem._data[0x0016] = 0b00100101;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b01001010, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b01001010);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b10100101")
			{
				mem._data[0x0016] = 0b10100101;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b01001010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b01001010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is asl [absolute]")
	{
		mem._data[0] = 0x0e;

		AND_GIVEN("address bytes are 0x1224")
		{
			mem._data[2] = 0x12;
			mem._data[1] = 0x24;

			AND_GIVEN("address 0x1224 value is 0b00000000")
			{
				mem._data[0x1224] = 0b00000000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b10000000")
			{
				mem._data[0x1224] = 0b10000000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b11100000")
			{
				mem._data[0x1224] = 0b11100000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b11000000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1224] == 0b11000000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b01100000")
			{
				mem._data[0x1224] = 0b01100000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b11000000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1224] == 0b11000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b00100101")
			{
				mem._data[0x1224] = 0b00100101;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b01001010, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b01001010);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b10100101")
			{
				mem._data[0x1224] = 0b10100101;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b01001010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b01001010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is asl [absolute, x]")
	{
		mem._data[0] = 0x1e;

		AND_GIVEN("address bytes are 0x10f0 and x register value is 0x12")
		{
			mem._data[2] = 0x10;
			mem._data[1] = 0xf0;
			cpu.x_register(0x12);

			AND_GIVEN("address 0x1102 value is 0b00000000")
			{
				mem._data[0x1102] = 0b00000000;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b10000000")
			{
				mem._data[0x1102] = 0b10000000;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b11100000")
			{
				mem._data[0x1102] = 0b11100000;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b11000000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1102] == 0b11000000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b01100000")
			{
				mem._data[0x1102] = 0b01100000;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b11000000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1102] == 0b11000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b00100101")
			{
				mem._data[0x1102] = 0b00100101;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b01001010, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b01001010);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b10100101")
			{
				mem._data[0x1102] = 0b10100101;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b01001010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b01001010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}
}

SCENARIO("Can perform lsr operation", "[cpu][shift_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);
	cpu.accumulator(0);

	GIVEN("operation is lsr [accumulator]")
	{
		mem._data[0] = 0x4a;

		AND_GIVEN("accumulator value is 0b00000000")
		{
			cpu.accumulator(0b00000000);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b00000000, "
					"c flag is not set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b00000000);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b00000001")
		{
			cpu.accumulator(0b00000001);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0x00, "
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x00000000);
					REQUIRE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b10100110")
		{
			cpu.accumulator(0b10100110);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b01010011, "
					"c flag is not set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b01010011);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is lsr [zero page]")
	{
		mem._data[0] = 0x46;

		AND_GIVEN("address byte is 0x12")
		{
			mem._data[1] = 0x12;

			AND_GIVEN("address 0x0012 value is 0b00000000")
			{
				mem._data[0x0012] = 0b00000000;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b00000001")
			{
				mem._data[0x0012] = 0b00000001;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b10100110")
			{
				mem._data[0x0012] = 0b10100110;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b01010011, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b01010011);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is lsr [zero page, x]")
	{
		mem._data[0] = 0x56;

		AND_GIVEN("address byte is 0x12 and x register value is 0x04")
		{
			mem._data[1] = 0x12;
			cpu.x_register(0x04);

			AND_GIVEN("address 0x0016 value is 0b00000000")
			{
				mem._data[0x0016] = 0b00000000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b00000001")
			{
				mem._data[0x0016] = 0b00000001;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b10100110")
			{
				mem._data[0x0016] = 0b10100110;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b01010011, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b01010011);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is lsr [absolute]")
	{
		mem._data[0] = 0x4e;

		AND_GIVEN("address bytes are 0x1224")
		{
			mem._data[2] = 0x12;
			mem._data[1] = 0x24;

			AND_GIVEN("address 0x1224 value is 0b00000000")
			{
				mem._data[0x1224] = 0b00000000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b00000001")
			{
				mem._data[0x1224] = 0b00000001;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b10100110")
			{
				mem._data[0x1224] = 0b10100110;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b01010011, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b01010011);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is lsr [absolute, x]")
	{
		mem._data[0] = 0x5e;

		AND_GIVEN("address bytes are 0x10f0 and x register value is 0x12")
		{
			mem._data[2] = 0x10;
			mem._data[1] = 0xf0;
			cpu.x_register(0x12);

			AND_GIVEN("address 0x1102 value is 0b00000000")
			{
				mem._data[0x1102] = 0b00000000;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b00000001")
			{
				mem._data[0x1102] = 0b00000001;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b10100110")
			{
				mem._data[0x1102] = 0b10100110;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b01010011, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b01010011);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}
}

SCENARIO("Can perform rol operation", "[cpu][shift_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is rol [accumulator]")
	{
		mem._data[0] = 0x2a;

		AND_GIVEN("accumulator value is 0b00000000 and c flag is not set")
		{
			cpu.accumulator(0b00000000);
			cpu.c_flag(false);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b00000000, "
					"c flag is not set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b00000000);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b10000000 and c flag is not set")
		{
			cpu.accumulator(0b10000000);
			cpu.c_flag(false);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0x00, "
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x00000000);
					REQUIRE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b11100000 and c flag is not set")
		{
			cpu.accumulator(0b11100000);
			cpu.c_flag(false);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b11000000, "
					"c flag is set, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0b11000000);
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b01100000 and c flag is not set")
		{
			cpu.accumulator(0b01100000);
			cpu.c_flag(false);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b11000000, "
					"c flag is not set, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0b11000000);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b00100101 and c flag is set")
		{
			cpu.accumulator(0b00100101);
			cpu.c_flag(true);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b01001011, "
					"c flag is not set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b01001011);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b10100101 and c flag is not set")
		{
			cpu.accumulator(0b10100101);
			cpu.c_flag(false);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b01001010, "
					"c flag is set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b01001010);
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is rol [zero page]")
	{
		mem._data[0] = 0x26;

		AND_GIVEN("address byte is 0x12")
		{
			mem._data[1] = 0x12;

			AND_GIVEN("address 0x0012 value is 0b00000000 and c flag is not set")
			{
				mem._data[0x0012] = 0b00000000;
				cpu.c_flag(false);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b10000000 and c flag is not set")
			{
				mem._data[0x0012] = 0b10000000;
				cpu.c_flag(false);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b11100000 and c flag is not set")
			{
				mem._data[0x0012] = 0b11100000;
				cpu.c_flag(false);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b11000000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0012] == 0b11000000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b01100000 and c flag is not set")
			{
				mem._data[0x0012] = 0b01100000;
				cpu.c_flag(false);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b11000000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0012] == 0b11000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b00100101 and c flag is set")
			{
				mem._data[0x0012] = 0b00100101;
				cpu.c_flag(true);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b01001011, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b01001011);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b10100101 and c flag is not set")
			{
				mem._data[0x0012] = 0b10100101;
				cpu.c_flag(false);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b01001010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b01001010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is rol [zero page, x]")
	{
		mem._data[0] = 0x36;

		AND_GIVEN("address byte is 0x12 and x register value is 0x04")
		{
			mem._data[1] = 0x12;
			cpu.x_register(0x04);

			AND_GIVEN("address 0x0016 value is 0b00000000 and c flag is not set")
			{
				mem._data[0x0016] = 0b00000000;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b10000000 and c flag is not set")
			{
				mem._data[0x0016] = 0b10000000;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b11100000 and c flag is not set")
			{
				mem._data[0x0016] = 0b11100000;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b11000000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0016] == 0b11000000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b01100000 and c flag is not set")
			{
				mem._data[0x0016] = 0b01100000;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b11000000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0016] == 0b11000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b00100101 and c flag is set")
			{
				mem._data[0x0016] = 0b00100101;
				cpu.c_flag(true);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b01001011, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b01001011);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b10100101 and c flag is not set")
			{
				mem._data[0x0016] = 0b10100101;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b01001010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b01001010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is rol [absolute]")
	{
		mem._data[0] = 0x2e;

		AND_GIVEN("address bytes are 0x1224")
		{
			mem._data[2] = 0x12;
			mem._data[1] = 0x24;

			AND_GIVEN("address 0x1224 value is 0b00000000 and c flag is not set")
			{
				mem._data[0x1224] = 0b00000000;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b10000000 and c flag is not set")
			{
				mem._data[0x1224] = 0b10000000;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b11100000 and c flag is not set")
			{
				mem._data[0x1224] = 0b11100000;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b11000000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1224] == 0b11000000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b01100000 and c flag is not set")
			{
				mem._data[0x1224] = 0b01100000;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b11000000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1224] == 0b11000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b00100101 and c flag is set")
			{
				mem._data[0x1224] = 0b00100101;
				cpu.c_flag(true);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b01001011, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b01001011);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b10100101 and c flag is not set")
			{
				mem._data[0x1224] = 0b10100101;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b01001010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b01001010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is rol [absolute, x]")
	{
		mem._data[0] = 0x3e;

		AND_GIVEN("address bytes are 0x10f0 and x register value is 0x12")
		{
			mem._data[2] = 0x10;
			mem._data[1] = 0xf0;
			cpu.x_register(0x12);

			AND_GIVEN("address 0x1102 value is 0b00000000 and c flag is not set")
			{
				mem._data[0x1102] = 0b00000000;
				cpu.c_flag(false);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b10000000 and c flag is not set")
			{
				mem._data[0x1102] = 0b10000000;
				cpu.c_flag(false);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b11100000 and c flag is not set")
			{
				mem._data[0x1102] = 0b11100000;
				cpu.c_flag(false);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b11000000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1102] == 0b11000000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b01100000 and c flag is not set")
			{
				mem._data[0x1102] = 0b01100000;
				cpu.c_flag(false);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b11000000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1102] == 0b11000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b00100101 and c flag is set")
			{
				mem._data[0x1102] = 0b00100101;
				cpu.c_flag(true);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b01001011, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b01001011);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b10100101 and c flag is not set")
			{
				mem._data[0x1102] = 0b10100101;
				cpu.c_flag(false);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b01001010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b01001010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}
}

SCENARIO("Can perform ror operation", "[cpu][shift_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is ror [accumulator]")
	{
		mem._data[0] = 0x6a;

		AND_GIVEN("accumulator value is 0b00000000 and c flag is not set")
		{
			cpu.accumulator(0b00000000);
			cpu.c_flag(false);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b00000000, "
					"c flag is not set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b00000000);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b00000001 and c flag is not set")
		{
			cpu.accumulator(0b00000001);
			cpu.c_flag(false);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0x00, "
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x00000000);
					REQUIRE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b11100001 and c flag is set")
		{
			cpu.accumulator(0b11100001);
			cpu.c_flag(true);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b11110000, "
					"c flag is set, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0b11110000);
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b01100000 and c flag is set")
		{
			cpu.accumulator(0b01100000);
			cpu.c_flag(true);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b10110000, "
					"c flag is not set, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0b10110000);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b10100100 and c flag is not set")
		{
			cpu.accumulator(0b10100100);
			cpu.c_flag(false);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b01010010, "
					"c flag is not set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b01010010);
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b10100101 and c flag is not set")
		{
			cpu.accumulator(0b10100101);
			cpu.c_flag(false);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b01010010, "
					"c flag is set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b01010010);
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is ror [zero page]")
	{
		mem._data[0] = 0x66;

		AND_GIVEN("address byte is 0x12")
		{
			mem._data[1] = 0x12;

			AND_GIVEN("address 0x0012 value is 0b00000000 and c flag is not set")
			{
				mem._data[0x0012] = 0b00000000;
				cpu.c_flag(false);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b00000001 and c flag is not set")
			{
				mem._data[0x0012] = 0b00000001;
				cpu.c_flag(false);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b11100001 and c flag is set")
			{
				mem._data[0x0012] = 0b11100001;
				cpu.c_flag(true);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b11110000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0012] == 0b11110000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b01100000 and c flag is set")
			{
				mem._data[0x0012] = 0b01100000;
				cpu.c_flag(true);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b10110000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0012] == 0b10110000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b10100100 and c flag is not set")
			{
				mem._data[0x0012] = 0b10100100;
				cpu.c_flag(false);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b01010010, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b01010010);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0012 value is 0b10100101 and c flag is not set")
			{
				mem._data[0x0012] = 0b10100101;
				cpu.c_flag(false);

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0012 has a value of 0b01010010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0012] == 0b01010010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is ror [zero page, x]")
	{
		mem._data[0] = 0x76;

		AND_GIVEN("address byte is 0x12 and x register value is 0x04")
		{
			mem._data[1] = 0x12;
			cpu.x_register(0x04);

			AND_GIVEN("address 0x0016 value is 0b00000000 and c flag is not set")
			{
				mem._data[0x0016] = 0b00000000;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b00000001 and c flag is not set")
			{
				mem._data[0x0016] = 0b00000001;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b11100001 and c flag is set")
			{
				mem._data[0x0016] = 0b11100001;
				cpu.c_flag(true);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b11110000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0016] == 0b11110000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b01100000 and c flag is set")
			{
				mem._data[0x0016] = 0b01100000;
				cpu.c_flag(true);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b10110000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0016] == 0b10110000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b10100100 and c flag is not set")
			{
				mem._data[0x0016] = 0b10100100;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b01010010, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b01010010);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0016 value is 0b10100101 and c flag is not set")
			{
				mem._data[0x0016] = 0b10100101;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0016 has a value of 0b01010010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0016] == 0b01010010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is ror [absolute]")
	{
		mem._data[0] = 0x6e;

		AND_GIVEN("address bytes are 0x1224")
		{
			mem._data[2] = 0x12;
			mem._data[1] = 0x24;

			AND_GIVEN("address 0x1224 value is 0b00000000 and c flag is not set")
			{
				mem._data[0x1224] = 0b00000000;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b00000001 and c flag is not set")
			{
				mem._data[0x1224] = 0b00000001;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b11100001 and c flag is set")
			{
				mem._data[0x1224] = 0b11100001;
				cpu.c_flag(true);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b11110000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1224] == 0b11110000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b01100000 and c flag is set")
			{
				mem._data[0x1224] = 0b01100000;
				cpu.c_flag(true);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b10110000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1224] == 0b10110000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b10100100 and c flag is not set")
			{
				mem._data[0x1224] = 0b10100100;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b01010010, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b01010010);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1224 value is 0b10100101 and c flag is not set")
			{
				mem._data[0x1224] = 0b10100101;
				cpu.c_flag(false);

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x1242 has a value of 0b01010010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1224] == 0b01010010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is ror [absolute, x]")
	{
		mem._data[0] = 0x7e;

		AND_GIVEN("address bytes are 0x10f0 and x register value is 0x12")
		{
			mem._data[2] = 0x10;
			mem._data[1] = 0xf0;
			cpu.x_register(0x12);

			AND_GIVEN("address 0x1102 value is 0b00000000 and c flag is not set")
			{
				mem._data[0x1102] = 0b00000000;
				cpu.c_flag(false);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b00000000, "
						"c flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b00000000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b00000001 and c flag is not set")
			{
				mem._data[0x1102] = 0b00000001;
				cpu.c_flag(false);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0x00, "
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0x00000000);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b11100001 and c flag is set")
			{
				mem._data[0x1102] = 0b11100001;
				cpu.c_flag(true);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b11110000, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1102] == 0b11110000);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b01100000 and c flag is set")
			{
				mem._data[0x1102] = 0b01100000;
				cpu.c_flag(true);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b10110000, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x1102] == 0b10110000);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b10100100 and c flag is not set")
			{
				mem._data[0x1102] = 0b10100100;
				cpu.c_flag(false);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b01010010, "
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b01010010);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x1102 value is 0b10100101 and c flag is not set")
			{
				mem._data[0x1102] = 0b10100101;
				cpu.c_flag(false);

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x1102 has a value of 0b01010010, "
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x1102] == 0b01010010);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}
}
