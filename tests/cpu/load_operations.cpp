#include <catch2/catch_test_macros.hpp>
#include <cpu/cpu.hpp>
#include <memory/slowMemory.hpp>

SCENARIO("Can perform lda operation", "[cpu][load_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);
	cpu.accumulator(0);

	GIVEN("operation is lda [immediate]")
	{
		mem._data[0] = 0xa9;

		AND_GIVEN("immediate value is 0x00")
		{
			mem._data[1] = 0x00;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("immediate value is 0x35")
		{
			mem._data[1] = 0x35;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN("accumulator has a value of 0x35, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x35);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("immediate value is 0x85")
		{
			mem._data[1] = 0x85;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN("accumulator has a value of 0x85, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0x85);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is lda [zero page] and address value is 0x13")
	{
		mem._data[0] = 0xa5;
		mem._data[1] = 0x13;

		AND_GIVEN("address 0x0013 has value of 0x00")
		{
			mem._data[0x0013] = 0x00;

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x0013 has value of 0x21")
		{
			mem._data[0x0013] = 0x21;

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN("accumulator has a value of 0x21, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x21);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x0013 has value of 0xaf")
		{
			mem._data[0x0013] = 0xaf;

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN("accumulator has a value of 0xaf, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0xaf);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is lda [zero page, x], address value is 0x13, and x register value is 0x10")
	{
		mem._data[0] = 0xb5;
		mem._data[1] = 0x13;
		cpu.x_register(0x10);

		AND_GIVEN("address 0x0023 has value of 0x00")
		{
			mem._data[0x0023] = 0x00;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x0023 has value of 0x21")
		{
			mem._data[0x0023] = 0x21;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("accumulator has a value of 0x21, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x21);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x0023 has value of 0xff")
		{
			mem._data[0x0023] = 0xff;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("accumulator has a value of 0xff, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0xff);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is lda [absolute] and address value is 0x2230")
	{
		mem._data[0] = 0xad;
		// address needs to be written in reverse order
		mem._data[2] = 0x22;
		mem._data[1] = 0x30;

		AND_GIVEN("address 0x2230 has value of 0x00")
		{
			mem._data[0x2230] = 0x00;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x2230 has value of 0x5b")
		{
			mem._data[0x2230] = 0x5b;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("accumulator has a value of 0x5b, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x5b);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x2230 has value of 0xab")
		{
			mem._data[0x2230] = 0xab;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("accumulator has a value of 0xab, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0xab);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is lda [absolute, x] and address value is 0x22fe")
	{
		mem._data[0] = 0xbd;
		// address needs to be written in reverse order
		mem._data[2] = 0x22;
		mem._data[1] = 0xfe;

		AND_GIVEN("x register value is 0x01")
		{
			cpu.x_register(0x01);

			AND_GIVEN("address 0x22ff has value of 0x00")
			{
				mem._data[0x22ff] = 0x00;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x00);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x22ff has value of 0x5f")
			{
				mem._data[0x22ff] = 0x5f;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0x5f, z flag is not set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x5f);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x22ff has value of 0xc6")
			{
				mem._data[0x22ff] = 0xc6;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN("accumulator has a value of 0xc6, z flag is not set, and n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0xc6);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}

		AND_GIVEN("x register value is 0x02")
		{
			cpu.x_register(0x02);

			AND_GIVEN("address 0x2300 has value of 0x00")
			{
				mem._data[0x2300] = 0x00;

				WHEN("operation is performed and page cross occurs (5 cycles)")
				{
					cpu.cycle(5);

					THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x00);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2300 has value of 0x5f")
			{
				mem._data[0x2300] = 0x5f;

				WHEN("operation is performed and page cross occurs (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"accumulator has a value of 0x5f, z flag is not set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x5f);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2300 has value of 0xc6")
			{
				mem._data[0x2300] = 0xc6;

				WHEN("operation is performed and page cross occurs (5 cycles)")
				{
					cpu.cycle(5);

					THEN("accumulator has a value of 0xc6, z flag is not set, and n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0xc6);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is lda [absolute, y] and address value is 0x22fe")
	{
		mem._data[0] = 0xb9;
		// address needs to be written in reverse order
		mem._data[2] = 0x22;
		mem._data[1] = 0xfe;

		AND_GIVEN("y register value is 0x01")
		{
			cpu.y_register(0x01);

			AND_GIVEN("address 0x22ff has value of 0x00")
			{
				mem._data[0x22ff] = 0x00;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x00);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x22ff has value of 0x5f")
			{
				mem._data[0x22ff] = 0x5f;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0x5f, z flag is not set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x5f);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x22ff has value of 0xc6")
			{
				mem._data[0x22ff] = 0xc6;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN("accumulator has a value of 0xc6, z flag is not set, and n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0xc6);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}

		AND_GIVEN("y register value is 0x02")
		{
			cpu.y_register(0x02);

			AND_GIVEN("address 0x2300 has value of 0x00")
			{
				mem._data[0x2300] = 0x00;

				WHEN("operation is performed and page cross occurs (5 cycles)")
				{
					cpu.cycle(5);

					THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x00);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2300 has value of 0x5f")
			{
				mem._data[0x2300] = 0x5f;

				WHEN("operation is performed and page cross occurs (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"accumulator has a value of 0x5f, z flag is not set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x5f);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2300 has value of 0xc6")
			{
				mem._data[0x2300] = 0xc6;

				WHEN("operation is performed and page cross occurs (5 cycles)")
				{
					cpu.cycle(5);

					THEN("accumulator has a value of 0xc6, z flag is not set, and n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0xc6);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN(
		"operation is lda [indirect, x], address value is 0x04, x register value is 0x04, and "
		"indirect address value is 0x0115")
	{
		mem._data[0] = 0xa1;
		mem._data[1] = 0x04;
		mem._data[0x08] = 0x15;
		mem._data[0x09] = 0x01;
		cpu.x_register(0x04);

		AND_GIVEN("address 0x0115 has a value of 0x00")
		{
			mem._data[0x0115] = 0x00;

			WHEN("operation is performed (6 cycles)")
			{
				cpu.cycle(6);

				THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x0115 has a value of 0x77")
		{
			mem._data[0x0115] = 0x77;

			WHEN("operation is performed (6 cycles)")
			{
				cpu.cycle(6);

				THEN("accumulator has a value of 0x77, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0x77);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x0115 has a value of 0x98")
		{
			mem._data[0x0115] = 0x98;

			WHEN("operation is performed (6 cycles)")
			{
				cpu.cycle(6);

				THEN("accumulator has a value of 0x98, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0x98);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN(
		"operation is lda [indirect, y], address value is 0x08 and indirect address value is "
		"0x01fe")
	{
		mem._data[0] = 0xb1;
		mem._data[1] = 0x08;
		mem._data[0x08] = 0xfe;
		mem._data[0x09] = 0x01;

		AND_GIVEN("and y register value is 0x01")
		{
			cpu.y_register(0x01);

			AND_GIVEN("address 0x01ff value has a value of 0x00")
			{
				mem._data[0x01ff] = 0x00;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x00);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x01ff value has a value of 0x3d")
			{
				mem._data[0x01ff] = 0x3d;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"accumulator has a value of 0x3d, z flag is not set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x3d);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x01ff value has a value of 0xea")
			{
				mem._data[0x01ff] = 0xea;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN("accumulator has a value of 0xea, z flag is not set, and n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0xea);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}

		AND_GIVEN("and y register value is 0x02")
		{
			cpu.y_register(0x02);

			AND_GIVEN("address 0x0200 value has a value of 0x00")
			{
				mem._data[0x0200] = 0x00;

				WHEN("operation is performed with page cross (6 cycles)")
				{
					cpu.cycle(6);

					THEN("accumulator has a value of 0x00, z flag is set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x00);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0200 value has a value of 0x3d")
			{
				mem._data[0x0200] = 0x3d;

				WHEN("operation is performed with page cross (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"accumulator has a value of 0x3d, z flag is not set, and n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0x3d);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0200 value has a value of 0xea")
			{
				mem._data[0x0200] = 0xea;

				WHEN("operation is performed with page cross (6 cycles)")
				{
					cpu.cycle(6);

					THEN("accumulator has a value of 0xea, z flag is not set, and n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0xea);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}
}

SCENARIO("Can perform ldx operation", "[cpu][load_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);
	cpu.x_register(0);

	GIVEN("operation is ldx [immediate]")
	{
		mem._data[0] = 0xa2;

		AND_GIVEN("immediate value is 0x00")
		{
			mem._data[1] = 0x00;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN("x register has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.x_register() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("immediate value is 0x35")
		{
			mem._data[1] = 0x35;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN("x register has a value of 0x35, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.x_register() == 0x35);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("immediate value is 0xd5")
		{
			mem._data[1] = 0xd5;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN("x register has a value of 0xd5, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.x_register() == 0xd5);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is ldx [zero page] and address value is 0x13")
	{
		mem._data[0] = 0xa6;
		mem._data[1] = 0x13;

		AND_GIVEN("address 0x13 has value of 0x00")
		{
			mem._data[0x0013] = 0x00;

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN("x register has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.x_register() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x13 has value of 0x21")
		{
			mem._data[0x0013] = 0x21;

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN("x register has a value of 0x21, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.x_register() == 0x21);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x13 has value of 0xf7")
		{
			mem._data[0x0013] = 0xf7;

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN("x register has a value of 0xf7, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.x_register() == 0xf7);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is ldx [zero page, y], address value is 0x13, and y register value is 0x10")
	{
		mem._data[0] = 0xb6;
		mem._data[1] = 0x13;
		cpu.y_register(0x10);

		AND_GIVEN("address 0x23 has value of 0x00")
		{
			mem._data[0x23] = 0x00;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("x register has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.x_register() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x23 has value of 0x21")
		{
			mem._data[0x23] = 0x21;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("x register has a value of 0x21, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.x_register() == 0x21);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x23 has value of 0x81")
		{
			mem._data[0x23] = 0x81;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("x register has a value of 0x81, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.x_register() == 0x81);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is ldx [absolute] and address vlue is 0x2230")
	{
		mem._data[0] = 0xae;
		// address needs to be written in reverse order
		mem._data[2] = 0x22;
		mem._data[1] = 0x30;

		AND_GIVEN("address 0x2230 has value of 0x00")
		{
			mem._data[0x2230] = 0x00;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("x register has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.x_register() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x2230 has value of 0x5b")
		{
			mem._data[0x2230] = 0x5b;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("x register has a value of 0x5b, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.x_register() == 0x5b);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x2230 has value of 0xe8")
		{
			mem._data[0x2230] = 0xe8;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("x register has a value of 0xe8, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.x_register() == 0xe8);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is ldx [absolute, y] and address value is 0x22fe")
	{
		mem._data[0] = 0xbe;
		// address needs to be written in reverse order
		mem._data[2] = 0x22;
		mem._data[1] = 0xfe;

		AND_GIVEN("y register value is 0x01")
		{
			cpu.y_register(0x01);

			AND_GIVEN("address 0x22ff has value of 0x00")
			{
				mem._data[0x22ff] = 0x00;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN("x register has a value of 0x00, z flag is set, and n flag is not set")
					{
						REQUIRE(cpu.x_register() == 0x00);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x22ff has value of 0x5f")
			{
				mem._data[0x22ff] = 0x5f;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN("x register has a value of 0x5f, z flag is not set, and n flag is not set")
					{
						REQUIRE(cpu.x_register() == 0x5f);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x22ff has value of 0xb4")
			{
				mem._data[0x22ff] = 0xb4;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN("x register has a value of 0xb4, z flag is not set, and n flag is set")
					{
						REQUIRE(cpu.x_register() == 0xb4);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}

		AND_GIVEN("y register value is 0x02")
		{
			cpu.y_register(0x02);

			AND_GIVEN("address 0x2300 has value of 0x00")
			{
				mem._data[0x2300] = 0x00;

				WHEN("operation is performed with page cross (5 cycles)")
				{
					cpu.cycle(5);

					THEN("x register has a value of 0x00, z flag is set, and n flag is not set")
					{
						REQUIRE(cpu.x_register() == 0x00);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2300 has value of 0x5f")
			{
				mem._data[0x2300] = 0x5f;

				WHEN("operation is performed with page cross (5 cycles)")
				{
					cpu.cycle(5);

					THEN("x register has a value of 0x5f, z flag is not set, and n flag is not set")
					{
						REQUIRE(cpu.x_register() == 0x5f);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2300 has value of 0xb4")
			{
				mem._data[0x2300] = 0xb4;

				WHEN("operation is performed with page cross (5 cycles)")
				{
					cpu.cycle(5);

					THEN("x register has a value of 0xb4, z flag is not set, and n flag is set")
					{
						REQUIRE(cpu.x_register() == 0xb4);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}
}

SCENARIO("Can perform ldy operation", "[cpu][load_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);
	cpu.y_register(0);

	GIVEN("operation is ldy [immediate]")
	{
		mem._data[0] = 0xa0;

		AND_GIVEN("immediate value is 0x00")
		{
			mem._data[1] = 0x00;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN("y register has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.y_register() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("immediate value is 0x35")
		{
			mem._data[1] = 0x35;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN("y register has a value of 0x35, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.y_register() == 0x35);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("immediate value is 0xd5")
		{
			mem._data[1] = 0xd5;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN("y register has a value of 0xd5, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.y_register() == 0xd5);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is ldy [zero page] and address value is 0x13")
	{
		mem._data[0] = 0xa4;
		mem._data[1] = 0x13;

		AND_GIVEN("address 0x13 has value of 0x00")
		{
			mem._data[0x0013] = 0x00;

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN("y register has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.y_register() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x13 has value of 0x21")
		{
			mem._data[0x0013] = 0x21;

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN("y register has a value of 0x21, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.y_register() == 0x21);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x13 has value of 0xf7")
		{
			mem._data[0x0013] = 0xf7;

			WHEN("operation is performed (3 cycles)")
			{
				cpu.cycle(3);

				THEN("y register has a value of 0xf7, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.y_register() == 0xf7);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is ldy [zero page, x], address value is 0x13, and x register value is 0x10")
	{
		mem._data[0] = 0xb4;
		mem._data[1] = 0x13;
		cpu.x_register(0x10);

		AND_GIVEN("address 0x23 has value of 0x00")
		{
			mem._data[0x23] = 0x00;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("y register has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.y_register() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x23 has value of 0x21")
		{
			mem._data[0x23] = 0x21;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("y register has a value of 0x21, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.y_register() == 0x21);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x23 has value of 0x81")
		{
			mem._data[0x23] = 0x81;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("y register has a value of 0x81, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.y_register() == 0x81);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is ldy [absolute] and address vlue is 0x2230")
	{
		mem._data[0] = 0xac;
		// address needs to be written in reverse order
		mem._data[2] = 0x22;
		mem._data[1] = 0x30;

		AND_GIVEN("address 0x2230 has value of 0x00")
		{
			mem._data[0x2230] = 0x00;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("y register has a value of 0x00, z flag is set, and n flag is not set")
				{
					REQUIRE(cpu.y_register() == 0x00);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x2230 has value of 0x5b")
		{
			mem._data[0x2230] = 0x5b;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("y register has a value of 0x5b, z flag is not set, and n flag is not set")
				{
					REQUIRE(cpu.y_register() == 0x5b);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("address 0x2230 has value of 0xe8")
		{
			mem._data[0x2230] = 0xe8;

			WHEN("operation is performed (4 cycles)")
			{
				cpu.cycle(4);

				THEN("y register has a value of 0xe8, z flag is not set, and n flag is set")
				{
					REQUIRE(cpu.y_register() == 0xe8);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is ldy [absolute, x] and address value is 0x22fe")
	{
		mem._data[0] = 0xbc;
		// address needs to be written in reverse order
		mem._data[2] = 0x22;
		mem._data[1] = 0xfe;

		AND_GIVEN("x register value is 0x01")
		{
			cpu.x_register(0x01);

			AND_GIVEN("address 0x22ff has value of 0x00")
			{
				mem._data[0x22ff] = 0x00;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN("y register has a value of 0x00, z flag is set, and n flag is not set")
					{
						REQUIRE(cpu.y_register() == 0x00);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x22ff has value of 0x5f")
			{
				mem._data[0x22ff] = 0x5f;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN("y register has a value of 0x5f, z flag is not set, and n flag is not set")
					{
						REQUIRE(cpu.y_register() == 0x5f);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x22ff has value of 0xb4")
			{
				mem._data[0x22ff] = 0xb4;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN("y register has a value of 0xb4, z flag is not set, and n flag is set")
					{
						REQUIRE(cpu.y_register() == 0xb4);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}

		AND_GIVEN("x register value is 0x02")
		{
			cpu.x_register(0x02);

			AND_GIVEN("address 0x2300 has value of 0x00")
			{
				mem._data[0x2300] = 0x00;

				WHEN("operation is performed with page cross (5 cycles)")
				{
					cpu.cycle(5);

					THEN("y register has a value of 0x00, z flag is set, and n flag is not set")
					{
						REQUIRE(cpu.y_register() == 0x00);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2300 has value of 0x5f")
			{
				mem._data[0x2300] = 0x5f;

				WHEN("operation is performed with page cross (5 cycles)")
				{
					cpu.cycle(5);

					THEN("y register has a value of 0x5f, z flag is not set, and n flag is not set")
					{
						REQUIRE(cpu.y_register() == 0x5f);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2300 has value of 0xb4")
			{
				mem._data[0x2300] = 0xb4;

				WHEN("operation is performed with page cross (5 cycles)")
				{
					cpu.cycle(5);

					THEN("y register has a value of 0xb4, z flag is not set, and n flag is set")
					{
						REQUIRE(cpu.y_register() == 0xb4);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}
}
