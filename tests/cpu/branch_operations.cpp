#include <catch2/catch_test_macros.hpp>
#include <cpu/cpu.hpp>

#include "../mockMemory.hpp"

SCENARIO("Can perform bcc operation", "[cpu][branch_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is bcc [relative]")
	{
		mem._data[0] = 0x90;

		AND_GIVEN("address value is 0x20")
		{
			mem._data[1] = 0x20;

			AND_GIVEN("c flag is not set")
			{
				cpu.c_flag(false);

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN("program counter has a value of 0x0022")
					{
						REQUIRE(cpu.program_counter() == 0x0022);
					}
				}
			}

			AND_GIVEN("c flag is set")
			{
				cpu.c_flag(true);

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN("program counter has a value of 0x0002")
					{
						REQUIRE(cpu.program_counter() == 0x0002);
					}
				}
			}
		}

		AND_GIVEN("address value is 0xff")
		{
			mem._data[1] = 0xff;

			AND_GIVEN("c flag is not set")
			{
				cpu.c_flag(false);

				WHEN("operation is performed with page cross (4 cycles)")
				{
					cpu.cycle(4);

					THEN("program counter has a value of 0x0101")
					{
						REQUIRE(cpu.program_counter() == 0x0101);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform bcs operation", "[cpu][branch_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is bcs [relative]")
	{
		mem._data[0] = 0xb0;

		AND_GIVEN("address value is 0x20")
		{
			mem._data[1] = 0x20;

			AND_GIVEN("c flag is not set")
			{
				cpu.c_flag(false);

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN("program counter has a value of 0x0002")
					{
						REQUIRE(cpu.program_counter() == 0x0002);
					}
				}
			}

			AND_GIVEN("c flag is set")
			{
				cpu.c_flag(true);

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN("program counter has a value of 0x0022")
					{
						REQUIRE(cpu.program_counter() == 0x0022);
					}
				}
			}
		}

		AND_GIVEN("address value is 0xff")
		{
			mem._data[1] = 0xff;

			AND_GIVEN("c flag is set")
			{
				cpu.c_flag(true);

				WHEN("operation is performed with page cross (4 cycles)")
				{
					cpu.cycle(4);

					THEN("program counter has a value of 0x0101")
					{
						REQUIRE(cpu.program_counter() == 0x0101);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform beq operation", "[cpu][branch_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is beq [relative]")
	{
		mem._data[0] = 0xf0;

		AND_GIVEN("address value is 0x20")
		{
			mem._data[1] = 0x20;

			AND_GIVEN("z flag is not set")
			{
				cpu.z_flag(false);

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN("program counter has a value of 0x0002")
					{
						REQUIRE(cpu.program_counter() == 0x0002);
					}
				}
			}

			AND_GIVEN("z flag is set")
			{
				cpu.z_flag(true);

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN("program counter has a value of 0x0022")
					{
						REQUIRE(cpu.program_counter() == 0x0022);
					}
				}
			}
		}

		AND_GIVEN("address value is 0xff")
		{
			mem._data[1] = 0xff;

			AND_GIVEN("z flag is set")
			{
				cpu.z_flag(true);

				WHEN("operation is performed with page cross (4 cycles)")
				{
					cpu.cycle(4);

					THEN("program counter has a value of 0x0101")
					{
						REQUIRE(cpu.program_counter() == 0x0101);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform bmi operation", "[cpu][branch_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is bmi [relative]")
	{
		mem._data[0] = 0x30;

		AND_GIVEN("address value is 0x20")
		{
			mem._data[1] = 0x20;

			AND_GIVEN("n flag is not set")
			{
				cpu.n_flag(false);

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN("program counter has a value of 0x0002")
					{
						REQUIRE(cpu.program_counter() == 0x0002);
					}
				}
			}

			AND_GIVEN("n flag is set")
			{
				cpu.n_flag(true);

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN("program counter has a value of 0x0022")
					{
						REQUIRE(cpu.program_counter() == 0x0022);
					}
				}
			}
		}

		AND_GIVEN("address value is 0xff")
		{
			mem._data[1] = 0xff;

			AND_GIVEN("n flag is set")
			{
				cpu.n_flag(true);

				WHEN("operation is performed with page cross (4 cycles)")
				{
					cpu.cycle(4);

					THEN("program counter has a value of 0x0101")
					{
						REQUIRE(cpu.program_counter() == 0x0101);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform bne operation", "[cpu][branch_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is bne [relative]")
	{
		mem._data[0] = 0xd0;

		AND_GIVEN("address value is 0x20")
		{
			mem._data[1] = 0x20;

			AND_GIVEN("z flag is set")
			{
				cpu.z_flag(true);

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN("program counter has a value of 0x0002")
					{
						REQUIRE(cpu.program_counter() == 0x0002);
					}
				}
			}

			AND_GIVEN("z flag is set")
			{
				cpu.z_flag(false);

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN("program counter has a value of 0x0022")
					{
						REQUIRE(cpu.program_counter() == 0x0022);
					}
				}
			}
		}

		AND_GIVEN("address value is 0xff")
		{
			mem._data[1] = 0xff;

			AND_GIVEN("z flag is set")
			{
				cpu.z_flag(false);

				WHEN("operation is performed with page cross (4 cycles)")
				{
					cpu.cycle(4);

					THEN("program counter has a value of 0x0101")
					{
						REQUIRE(cpu.program_counter() == 0x0101);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform bpl operation", "[cpu][branch_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is bpl [relative]")
	{
		mem._data[0] = 0x10;

		AND_GIVEN("address value is 0x20")
		{
			mem._data[1] = 0x20;

			AND_GIVEN("n flag is set")
			{
				cpu.n_flag(true);

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN("program counter has a value of 0x0002")
					{
						REQUIRE(cpu.program_counter() == 0x0002);
					}
				}
			}

			AND_GIVEN("n flag is set")
			{
				cpu.n_flag(false);

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN("program counter has a value of 0x0022")
					{
						REQUIRE(cpu.program_counter() == 0x0022);
					}
				}
			}
		}

		AND_GIVEN("address value is 0xff")
		{
			mem._data[1] = 0xff;

			AND_GIVEN("n flag is set")
			{
				cpu.n_flag(false);

				WHEN("operation is performed with page cross (4 cycles)")
				{
					cpu.cycle(4);

					THEN("program counter has a value of 0x0101")
					{
						REQUIRE(cpu.program_counter() == 0x0101);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform bvc operation", "[cpu][branch_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is bvc [relative]")
	{
		mem._data[0] = 0x50;

		AND_GIVEN("address value is 0x20")
		{
			mem._data[1] = 0x20;

			AND_GIVEN("v flag is not set")
			{
				cpu.v_flag(false);

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN("program counter has a value of 0x0022")
					{
						REQUIRE(cpu.program_counter() == 0x0022);
					}
				}
			}

			AND_GIVEN("v flag is set")
			{
				cpu.v_flag(true);

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN("program counter has a value of 0x0002")
					{
						REQUIRE(cpu.program_counter() == 0x0002);
					}
				}
			}
		}

		AND_GIVEN("address value is 0xff")
		{
			mem._data[1] = 0xff;

			AND_GIVEN("v flag is not set")
			{
				cpu.v_flag(false);

				WHEN("operation is performed with page cross (4 cycles)")
				{
					cpu.cycle(4);

					THEN("program counter has a value of 0x0101")
					{
						REQUIRE(cpu.program_counter() == 0x0101);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform bvs operation", "[cpu][branch_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is bvs [relative]")
	{
		mem._data[0] = 0x70;

		AND_GIVEN("address value is 0x20")
		{
			mem._data[1] = 0x20;

			AND_GIVEN("v flag is not set")
			{
				cpu.v_flag(false);

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN("program counter has a value of 0x0002")
					{
						REQUIRE(cpu.program_counter() == 0x0002);
					}
				}
			}

			AND_GIVEN("v flag is set")
			{
				cpu.v_flag(true);

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN("program counter has a value of 0x0022")
					{
						REQUIRE(cpu.program_counter() == 0x0022);
					}
				}
			}
		}

		AND_GIVEN("address value is 0xff")
		{
			mem._data[1] = 0xff;

			AND_GIVEN("v flag is set")
			{
				cpu.v_flag(true);

				WHEN("operation is performed with page cross (4 cycles)")
				{
					cpu.cycle(4);

					THEN("program counter has a value of 0x0101")
					{
						REQUIRE(cpu.program_counter() == 0x0101);
					}
				}
			}
		}
	}
}

SCENARIO("Can perform jmp operation", "[cpu][branch_ops]")
{
	MockMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is jmp [absolute]")
	{
		mem._data[0] = 0x4c;

		AND_GIVEN("address value is 0x2056")
		{
			mem._data[2] = 0x20;
			mem._data[1] = 0x56;

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN("program counter has a value of 0x2056")
				{
					REQUIRE(cpu.program_counter() == 0x2056);
				}
			}
		}
	}

	GIVEN("operation is jmp [indirect]")
	{
		mem._data[0] = 0x6c;

		AND_GIVEN("address value is 0x2056 and indirect address value is 0x1772")
		{
			mem._data[2] = 0x20;
			mem._data[1] = 0x56;
			mem._data[0x2056] = 0x72;
			mem._data[0x2057] = 0x17;

			WHEN("operation is performed (5 cycles)")
			{
				cpu.cycle(5);

				THEN("program counter has a value of 0x1772")
				{
					REQUIRE(cpu.program_counter() == 0x1772);
				}
			}
		}

		AND_GIVEN("address value is 0x01ff and indirect address value is 0x1682")
		{
			mem._data[2] = 0x01;
			mem._data[1] = 0xff;
			mem._data[0x01ff] = 0x82;
			mem._data[0x0200] = 0x17;
			mem._data[0x0100] = 0x16;

			WHEN("operation is performed with page cross (5 cycles)")
			{
				cpu.cycle(5);

				THEN("program counter has a value of 0x1682")
				{
					REQUIRE(cpu.program_counter() == 0x1682);
				}
			}
		}
	}
}
