#include <catch2/catch_test_macros.hpp>
#include <cpu/cpu.hpp>
#include <memory/slowMemory.hpp>

SCENARIO("Can perform and operation", "[cpu][logical_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is and [immediate]")
	{
		mem._data[0] = 0x29;

		AND_GIVEN("accumulator value is 0b10101010 and op value is 0b11110000")
		{
			cpu.accumulator(0b10101010);
			mem._data[1] = 0b11110000;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b10100000, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0b10100000);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b10000000 and op value is 0b01010101")
		{
			cpu.accumulator(0b10000000);
			mem._data[1] = 0b01010101;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b00000000, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b00000000);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b00001111 and op value is 0b01010101")
		{
			cpu.accumulator(0b00001111);
			mem._data[1] = 0b01010101;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b00000101, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b00000101);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is and [zero page]")
	{
		mem._data[0] = 0x25;

		AND_GIVEN("address value is 0x27")
		{
			mem._data[1] = 0x27;

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x0027 value is 0b11110000")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x0027] = 0b11110000;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b10100000, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10100000);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b10000000 and "
				"address 0x0027 value is 0b01010101")
			{
				cpu.accumulator(0b10000000);
				mem._data[0x0027] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x0027 value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x0027] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000101, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000101);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is and [zero page, x]")
	{
		mem._data[0] = 0x35;

		AND_GIVEN("address value is 0x27 and x register value is 0x15")
		{
			mem._data[1] = 0x27;
			cpu.x_register(0x15);

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x003c value is 0b11110000")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x003c] = 0b11110000;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b10100000, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10100000);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b10000000 and "
				"address 0x003c value is 0b01010101")
			{
				cpu.accumulator(0b10000000);
				mem._data[0x003c] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x003c value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x003c] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000101, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000101);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is and [absolute]")
	{
		mem._data[0] = 0x2d;

		AND_GIVEN("address value is 0x1527")
		{
			mem._data[2] = 0x15;
			mem._data[1] = 0x27;

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x1527 value is 0b11110000")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x1527] = 0b11110000;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b10100000, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10100000);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b10000000 and "
				"address 0x1527 value is 0b01010101")
			{
				cpu.accumulator(0b10000000);
				mem._data[0x1527] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x1527 value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x1527] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000101, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000101);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is and [absolute, x]")
	{
		mem._data[0] = 0x3d;

		AND_GIVEN("address value is 0x11fe")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0xfe;

			AND_GIVEN("x register value is 0x01")
			{
				cpu.x_register(0x01);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x11ff value is 0b11110000")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x11ff] = 0b11110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b10100000, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100000);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b10000000 and "
					"address 0x11ff value is 0b01010101")
				{
					cpu.accumulator(0b10000000);
					mem._data[0x11ff] = 0b01010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x11ff value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x11ff] = 0b01010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b00000101, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("x register value is 0x11")
			{
				cpu.x_register(0x11);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x120f value is 0b11110000")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x120f] = 0b11110000;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b10100000, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100000);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b10000000 and "
					"address 0x120f value is 0b01010101")
				{
					cpu.accumulator(0b10000000);
					mem._data[0x120f] = 0b01010101;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x120f value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x120f] = 0b01010101;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000101, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is and [absolute, y]")
	{
		mem._data[0] = 0x39;

		AND_GIVEN("address value is 0x11fe")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0xfe;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x11ff value is 0b11110000")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x11ff] = 0b11110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b10100000, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100000);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b10000000 and "
					"address 0x11ff value is 0b01010101")
				{
					cpu.accumulator(0b10000000);
					mem._data[0x11ff] = 0b01010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x11ff value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x11ff] = 0b01010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b00000101, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x11")
			{
				cpu.y_register(0x11);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x120f value is 0b11110000")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x120f] = 0b11110000;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b10100000, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100000);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b10000000 and "
					"address 0x120f value is 0b01010101")
				{
					cpu.accumulator(0b10000000);
					mem._data[0x120f] = 0b01010101;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x120f value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x120f] = 0b01010101;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000101, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is and [indirect, x]")
	{
		mem._data[0] = 0x21;

		AND_GIVEN(
			"address value is 0x04, "
			"x register value is 0x04, and "
			"indirect address value is 0x0115")
		{
			mem._data[1] = 0x04;
			mem._data[0x08] = 0x15;
			mem._data[0x09] = 0x01;
			cpu.x_register(0x04);

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x0115 value is 0b11110000")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x0115] = 0b11110000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"accumulator has a value of 0b10100000, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10100000);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b10000000 and "
				"address 0x0115 value is 0b01010101")
			{
				cpu.accumulator(0b10000000);
				mem._data[0x0115] = 0b01010101;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x0115 value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x0115] = 0b01010101;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"accumulator has a value of 0b00000101, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000101);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is and [indirect, y]")
	{
		mem._data[0] = 0x31;

		AND_GIVEN("address value is 0x08 and indirect address value is 0x01fe")
		{
			mem._data[1] = 0x08;
			mem._data[0x08] = 0xfe;
			mem._data[0x09] = 0x01;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x01ff value is 0b11110000")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x01ff] = 0b11110000;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b10100000, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100000);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b10000000 and "
					"address 0x01ff value is 0b01010101")
				{
					cpu.accumulator(0b10000000);
					mem._data[0x01ff] = 0b01010101;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x01ff value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x01ff] = 0b01010101;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000101, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x02")
			{
				cpu.y_register(0x02);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x0200 value is 0b11110000")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x0200] = 0b11110000;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0b10100000, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100000);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b10000000 and "
					"address 0x0200 value is 0b01010101")
				{
					cpu.accumulator(0b10000000);
					mem._data[0x0200] = 0b01010101;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x0200 value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x0200] = 0b01010101;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0b00000101, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}
}

SCENARIO("Can perform bit operation", "[cpu][logical_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is bit [zero page]")
	{
		mem._data[0] = 0x24;

		AND_GIVEN("address value is 0x27")
		{
			mem._data[1] = 0x27;

			AND_GIVEN("accumulator value is 0b00100000")
			{
				cpu.accumulator(0b00100000);

				AND_GIVEN("address 0x0027 value is 0b11110000")
				{
					mem._data[0x0027] = 0b11110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is set, and "
							"v flag is set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
							REQUIRE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0027 value is 0b01110000")
				{
					mem._data[0x0027] = 0b01110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is not set, and "
							"v flag is set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
							REQUIRE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0027 value is 0b10110000")
				{
					mem._data[0x0027] = 0b10110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is set, and "
							"v flag is not set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0027 value is 0b00110000")
				{
					mem._data[0x0027] = 0b00110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is not set, and "
							"v flag is not set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0027 value is 0b10010000")
				{
					mem._data[0x0027] = 0b10010000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is set, "
							"n flag is set, and "
							"v flag is not set")
						{
							REQUIRE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}
			}

			AND_GIVEN("accumulator value is 0b10101010")
			{
				cpu.accumulator(0b10101010);

				AND_GIVEN("address 0x0027 value is 0b00110000")
				{
					mem._data[0x0027] = 0b00110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is not set, and "
							"v flag is not set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0027 value is 0b00010101")
				{
					mem._data[0x0027] = 0b00010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is set, "
							"n flag is not set, and "
							"v flag is not set")
						{
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0027 value is 0b00000010")
				{
					mem._data[0x0027] = 0b00110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is not set, and "
							"v flag is not set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is bit [absolute]")
	{
		mem._data[0] = 0x2c;

		AND_GIVEN("address value is 0x0227")
		{
			mem._data[2] = 0x02;
			mem._data[1] = 0x27;

			AND_GIVEN("accumulator value is 0b00100000")
			{
				cpu.accumulator(0b00100000);

				AND_GIVEN("address 0x0227 value is 0b11110000")
				{
					mem._data[0x0227] = 0b11110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is set, and "
							"v flag is set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
							REQUIRE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0227 value is 0b01110000")
				{
					mem._data[0x0227] = 0b01110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is not set, and "
							"v flag is set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
							REQUIRE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0227 value is 0b10110000")
				{
					mem._data[0x0227] = 0b10110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is set, and "
							"v flag is not set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0227 value is 0b00110000")
				{
					mem._data[0x0227] = 0b00110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is not set, and "
							"v flag is not set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0227 value is 0b10010000")
				{
					mem._data[0x0227] = 0b10010000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is set, "
							"n flag is set, and "
							"v flag is not set")
						{
							REQUIRE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}
			}

			AND_GIVEN("accumulator value is 0b10101010")
			{
				cpu.accumulator(0b10101010);

				AND_GIVEN("address 0x0227 value is 0b00110000")
				{
					mem._data[0x0227] = 0b00110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is not set, and "
							"v flag is not set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0227 value is 0b00010101")
				{
					mem._data[0x0227] = 0b00010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is set, "
							"n flag is not set, and "
							"v flag is not set")
						{
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}

				AND_GIVEN("address 0x0227 value is 0b00000010")
				{
					mem._data[0x0227] = 0b00110000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"z flag is not set, "
							"n flag is not set, and "
							"v flag is not set")
						{
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
							REQUIRE_FALSE(cpu.v_flag());
						}
					}
				}
			}
		}
	}
}

SCENARIO("Can perform cmp operation", "[cpu][logical_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is cmp [immediate]")
	{
		mem._data[0] = 0xc9;

		AND_GIVEN("accumulator value is 7 and op value is 1")
		{
			cpu.accumulator(7);
			mem._data[1] = 1;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 7 and op value is -1")
		{
			cpu.accumulator(7);
			mem._data[1] = -1;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is not set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					// The c flag is not set based on a signed comparison, so a negative number will
					// appear larger than a positive number
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 7 and op value is 7")
		{
			cpu.accumulator(7);
			mem._data[1] = 7;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 7 and op value is 11")
		{
			cpu.accumulator(7);
			mem._data[1] = 11;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is -2 and op value is -6")
		{
			cpu.accumulator(-2);
			mem._data[1] = -6;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is cmp [zero page]")
	{
		mem._data[0] = 0xc5;

		AND_GIVEN("address value is 0x11")
		{
			mem._data[1] = 0x11;

			AND_GIVEN("accumulator value is 7 and address 0x0011 value is 1")
			{
				cpu.accumulator(7);
				mem._data[0x0011] = 1;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x0011 value is -1")
			{
				cpu.accumulator(7);
				mem._data[0x0011] = -1;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						// The c flag is not set based on a signed comparison, so a negative number
						// will appear larger than a positive number
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x0011 value is 7")
			{
				cpu.accumulator(7);
				mem._data[0x0011] = 7;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x0011 value is 11")
			{
				cpu.accumulator(7);
				mem._data[0x0011] = 11;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is -2 and address 0x0011 value is -6")
			{
				cpu.accumulator(-2);
				mem._data[0x0011] = -6;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is cmp [zero page, x]")
	{
		mem._data[0] = 0xd5;

		AND_GIVEN("address value is 0x0f and x register value is 0x2")
		{
			mem._data[1] = 0x0f;
			cpu.x_register(0x02);

			AND_GIVEN("accumulator value is 7 and address 0x0011 value is 1")
			{
				cpu.accumulator(7);
				mem._data[0x0011] = 1;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x0011 value is -1")
			{
				cpu.accumulator(7);
				mem._data[0x0011] = -1;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						// The c flag is not set based on a signed comparison, so a negative number
						// will appear larger than a positive number
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x0011 value is 7")
			{
				cpu.accumulator(7);
				mem._data[0x0011] = 7;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x0011 value is 11")
			{
				cpu.accumulator(7);
				mem._data[0x0011] = 11;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is -2 and address 0x0011 value is -6")
			{
				cpu.accumulator(-2);
				mem._data[0x0011] = -6;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is cmp [absolute]")
	{
		mem._data[0] = 0xcd;

		AND_GIVEN("address value is 0x1111")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0x11;

			AND_GIVEN("accumulator value is 7 and address 0x1111 value is 1")
			{
				cpu.accumulator(7);
				mem._data[0x1111] = 1;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x1111 value is -1")
			{
				cpu.accumulator(7);
				mem._data[0x1111] = -1;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						// The c flag is not set based on a signed comparison, so a negative number
						// will appear larger than a positive number
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x1111 value is 7")
			{
				cpu.accumulator(7);
				mem._data[0x1111] = 7;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x1111 value is 11")
			{
				cpu.accumulator(7);
				mem._data[0x1111] = 11;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is -2 and address 0x1111 value is -6")
			{
				cpu.accumulator(-2);
				mem._data[0x1111] = -6;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is cmp [absolute, x]")
	{
		mem._data[0] = 0xdd;

		AND_GIVEN("address value is 0x01fe")
		{
			mem._data[2] = 0x01;
			mem._data[1] = 0xfe;

			AND_GIVEN("x register value is 0x01")
			{
				cpu.x_register(0x01);

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is 1")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = 1;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"c flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is -1")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = -1;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"c flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							// The c flag is not set based on a signed comparison, so a negative
							// number will appear larger than a positive number
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is 7")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = 7;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is 11")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = 11;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is -2 and address 0x01ff value is -6")
				{
					cpu.accumulator(-2);
					mem._data[0x01ff] = -6;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("x register value is 0x02")
			{
				cpu.x_register(0x02);

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is 1")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = 1;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is -1")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = -1;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							// The c flag is not set based on a signed comparison, so a negative
							// number will appear larger than a positive number
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is 7")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = 7;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is 11")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = 11;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is -2 and address 0x0200 value is -6")
				{
					cpu.accumulator(-2);
					mem._data[0x0200] = -6;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is cmp [absolute, y]")
	{
		mem._data[0] = 0xd9;

		AND_GIVEN("address value is 0x01fe")
		{
			mem._data[2] = 0x01;
			mem._data[1] = 0xfe;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is 1")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = 1;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"c flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is -1")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = -1;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"c flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							// The c flag is not set based on a signed comparison, so a negative
							// number will appear larger than a positive number
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is 7")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = 7;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is 11")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = 11;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is -2 and address 0x01ff value is -6")
				{
					cpu.accumulator(-2);
					mem._data[0x01ff] = -6;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x02")
			{
				cpu.y_register(0x02);

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is 1")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = 1;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is -1")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = -1;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							// The c flag is not set based on a signed comparison, so a negative
							// number will appear larger than a positive number
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is 7")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = 7;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is 11")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = 11;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is -2 and address 0x0200 value is -6")
				{
					cpu.accumulator(-2);
					mem._data[0x0200] = -6;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is cmp [indirect, x]")
	{
		mem._data[0] = 0xc1;

		AND_GIVEN(
			"address value is 0x0f, "
			"x register value is 0x01, and "
			"indirect address value is 0x01ff")
		{
			mem._data[1] = 0x0f;
			mem._data[0x0010] = 0xff;
			mem._data[0x0011] = 0x01;
			cpu.x_register(0x01);

			AND_GIVEN("accumulator value is 7 and address 0x01ff value is 1")
			{
				cpu.accumulator(7);
				mem._data[0x01ff] = 1;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x01ff value is -1")
			{
				cpu.accumulator(7);
				mem._data[0x01ff] = -1;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						// The c flag is not set based on a signed comparison, so a negative
						// number will appear larger than a positive number
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x01ff value is 7")
			{
				cpu.accumulator(7);
				mem._data[0x01ff] = 7;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 7 and address 0x01ff value is 11")
			{
				cpu.accumulator(7);
				mem._data[0x01ff] = 11;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is -2 and address 0x01ff value is -6")
			{
				cpu.accumulator(-2);
				mem._data[0x01ff] = -6;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is cmp [indirect, y]")
	{
		mem._data[0] = 0xd1;

		AND_GIVEN("address value is 0x08 and indirect address value is 0x01fe")
		{
			mem._data[1] = 0x08;
			mem._data[0x08] = 0xfe;
			mem._data[0x09] = 0x01;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is 1")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = 1;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is -1")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = -1;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							// The c flag is not set based on a signed comparison, so a negative
							// number will appear larger than a positive number
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is 7")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = 7;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x01ff value is 11")
				{
					cpu.accumulator(7);
					mem._data[0x01ff] = 11;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is -2 and address 0x01ff value is -6")
				{
					cpu.accumulator(-2);
					mem._data[0x01ff] = -6;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x02")
			{
				cpu.y_register(0x02);

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is 1")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = 1;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"c flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is -1")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = -1;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"c flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							// The c flag is not set based on a signed comparison, so a negative
							// number will appear larger than a positive number
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is 7")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = 7;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is 7 and address 0x0200 value is 11")
				{
					cpu.accumulator(7);
					mem._data[0x0200] = 11;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN("accumulator value is -2 and address 0x0200 value is -6")
				{
					cpu.accumulator(-2);
					mem._data[0x0200] = -6;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"c flag is set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}
}

SCENARIO("Can perform cpx operation", "[cpu][logical_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is cpx [immediate]")
	{
		mem._data[0] = 0xe0;

		AND_GIVEN("x register value is 7 and op value is 1")
		{
			cpu.x_register(7);
			mem._data[1] = 1;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("x register value is 7 and op value is -1")
		{
			cpu.x_register(7);
			mem._data[1] = -1;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is not set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					// The c flag is not set based on a signed comparison, so a negative number will
					// appear larger than a positive number
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("x register value is 7 and op value is 7")
		{
			cpu.x_register(7);
			mem._data[1] = 7;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("x register value is 7 and op value is 11")
		{
			cpu.x_register(7);
			mem._data[1] = 11;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("x register value is -2 and op value is -6")
		{
			cpu.x_register(-2);
			mem._data[1] = -6;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is cpx [zero page]")
	{
		mem._data[0] = 0xe4;

		AND_GIVEN("address value is 0x11")
		{
			mem._data[1] = 0x11;

			AND_GIVEN("x register value is 7 and address 0x0011 value is 1")
			{
				cpu.x_register(7);
				mem._data[0x0011] = 1;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("x register value is 7 and address 0x0011 value is -1")
			{
				cpu.x_register(7);
				mem._data[0x0011] = -1;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						// The c flag is not set based on a signed comparison, so a negative number
						// will appear larger than a positive number
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("x register value is 7 and address 0x0011 value is 7")
			{
				cpu.x_register(7);
				mem._data[0x0011] = 7;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("x register value is 7 and address 0x0011 value is 11")
			{
				cpu.x_register(7);
				mem._data[0x0011] = 11;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("x register value is -2 and address 0x0011 value is -6")
			{
				cpu.x_register(-2);
				mem._data[0x0011] = -6;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is cpx [absolute]")
	{
		mem._data[0] = 0xec;

		AND_GIVEN("address value is 0x1111")
		{
			mem._data[1] = 0x11;
			mem._data[2] = 0x11;

			AND_GIVEN("x register value is 7 and address 0x1111 value is 1")
			{
				cpu.x_register(7);
				mem._data[0x1111] = 1;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("x register value is 7 and address 0x1111 value is -1")
			{
				cpu.x_register(7);
				mem._data[0x1111] = -1;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						// The c flag is not set based on a signed comparison, so a negative number
						// will appear larger than a positive number
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("x register value is 7 and address 0x1111 value is 7")
			{
				cpu.x_register(7);
				mem._data[0x1111] = 7;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("x register value is 7 and address 0x1111 value is 11")
			{
				cpu.x_register(7);
				mem._data[0x1111] = 11;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("x register value is -2 and address 0x1111 value is -6")
			{
				cpu.x_register(-2);
				mem._data[0x1111] = -6;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}
}

SCENARIO("Can perform cpy operation", "[cpu][logical_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is cpy [immediate]")
	{
		mem._data[0] = 0xc0;

		AND_GIVEN("y register value is 7 and op value is 1")
		{
			cpu.y_register(7);
			mem._data[1] = 1;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("y register value is 7 and op value is -1")
		{
			cpu.y_register(7);
			mem._data[1] = -1;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is not set, "
					"z flag is not set, and "
					"n flag is not set")
				{
					// The c flag is not set based on a signed comparison, so a negative number will
					// appear larger than a positive number
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("y register value is 7 and op value is 7")
		{
			cpu.y_register(7);
			mem._data[1] = 7;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.c_flag());
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("y register value is 7 and op value is 11")
		{
			cpu.y_register(7);
			mem._data[1] = 11;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE_FALSE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("y register value is -2 and op value is -6")
		{
			cpu.y_register(-2);
			mem._data[1] = -6;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"c flag is set, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.c_flag());
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is cpy [zero page]")
	{
		mem._data[0] = 0xc4;

		AND_GIVEN("address value is 0x11")
		{
			mem._data[1] = 0x11;

			AND_GIVEN("y register value is 7 and address 0x0011 value is 1")
			{
				cpu.y_register(7);
				mem._data[0x0011] = 1;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("y register value is 7 and address 0x0011 value is -1")
			{
				cpu.y_register(7);
				mem._data[0x0011] = -1;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						// The c flag is not set based on a signed comparison, so a negative number
						// will appear larger than a positive number
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("y register value is 7 and address 0x0011 value is 7")
			{
				cpu.y_register(7);
				mem._data[0x0011] = 7;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("y register value is 7 and address 0x0011 value is 11")
			{
				cpu.y_register(7);
				mem._data[0x0011] = 11;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("y register value is -2 and address 0x0011 value is -6")
			{
				cpu.y_register(-2);
				mem._data[0x0011] = -6;

				WHEN("operation is performed (3 cycles)")
				{
					cpu.cycle(3);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is cpy [absolute]")
	{
		mem._data[0] = 0xcc;

		AND_GIVEN("address value is 0x1111")
		{
			mem._data[1] = 0x11;
			mem._data[2] = 0x11;

			AND_GIVEN("y register value is 7 and address 0x1111 value is 1")
			{
				cpu.y_register(7);
				mem._data[0x1111] = 1;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("y register value is 7 and address 0x1111 value is -1")
			{
				cpu.y_register(7);
				mem._data[0x1111] = -1;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						// The c flag is not set based on a signed comparison, so a negative number
						// will appear larger than a positive number
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("y register value is 7 and address 0x1111 value is 7")
			{
				cpu.y_register(7);
				mem._data[0x1111] = 7;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("y register value is 7 and address 0x1111 value is 11")
			{
				cpu.y_register(7);
				mem._data[0x1111] = 11;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("y register value is -2 and address 0x1111 value is -6")
			{
				cpu.y_register(-2);
				mem._data[0x1111] = -6;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"c flag is set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}
}

SCENARIO("Can perform eor operation", "[cpu][logical_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is eor [immediate]")
	{
		mem._data[0] = 0x49;

		AND_GIVEN("accumulator value is 0b10101010 and op value is 0b00001111")
		{
			cpu.accumulator(0b10101010);
			mem._data[1] = 0b00001111;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b10100101, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0b10100101);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b01010101 and op value is 0b01010101")
		{
			cpu.accumulator(0b01010101);
			mem._data[1] = 0b01010101;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b00000000, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b00000000);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b00001111 and op value is 0b01010101")
		{
			cpu.accumulator(0b00001111);
			mem._data[1] = 0b01010101;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b01011010, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b01011010);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is eor [zero page]")
	{
		mem._data[0] = 0x45;

		AND_GIVEN("address value is 0x27")
		{
			mem._data[1] = 0x27;

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x0027 value is 0b00001111")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x0027] = 0b00001111;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b10100101, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10100101);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b01010101 and "
				"address 0x0027 value is 0b01010101")
			{
				cpu.accumulator(0b01010101);
				mem._data[0x0027] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x0027 value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x0027] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b01011010, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b01011010);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is eor [zero page, x]")
	{
		mem._data[0] = 0x55;

		AND_GIVEN("address value is 0x27 and x register value is 0x15")
		{
			mem._data[1] = 0x27;
			cpu.x_register(0x15);

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x003c value is 0b00001111")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x003c] = 0b00001111;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b10100101, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10100101);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b01010101 and "
				"address 0x003c value is 0b01010101")
			{
				cpu.accumulator(0b01010101);
				mem._data[0x003c] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x003c value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x003c] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b01011010, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b01011010);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is eor [absolute]")
	{
		mem._data[0] = 0x4d;

		AND_GIVEN("address value is 0x1527")
		{
			mem._data[2] = 0x15;
			mem._data[1] = 0x27;

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x1527 value is 0b00001111")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x1527] = 0b00001111;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b10100101, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10100101);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b01010101 and "
				"address 0x1527 value is 0b01010101")
			{
				cpu.accumulator(0b01010101);
				mem._data[0x1527] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x1527 value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x1527] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b01011010, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b01011010);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is eor [absolute, x]")
	{
		mem._data[0] = 0x5d;

		AND_GIVEN("address value is 0x11fe")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0xfe;

			AND_GIVEN("x register value is 0x01")
			{
				cpu.x_register(0x01);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x11ff value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x11ff] = 0b00001111;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b10100101, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b01010101 and "
					"address 0x11ff value is 0b01010101")
				{
					cpu.accumulator(0b01010101);
					mem._data[0x11ff] = 0b01010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x11ff value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x11ff] = 0b01010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b01011010, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011010);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("x register value is 0x11")
			{
				cpu.x_register(0x11);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x120f value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x120f] = 0b00001111;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b10100101, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b01010101 and "
					"address 0x120f value is 0b01010101")
				{
					cpu.accumulator(0b01010101);
					mem._data[0x120f] = 0b01010101;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x120f value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x120f] = 0b01010101;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b01011010, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011010);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is eor [absolute, y]")
	{
		mem._data[0] = 0x59;

		AND_GIVEN("address value is 0x11fe")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0xfe;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x11ff value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x11ff] = 0b00001111;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b10100101, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b01010101 and "
					"address 0x11ff value is 0b01010101")
				{
					cpu.accumulator(0b01010101);
					mem._data[0x11ff] = 0b01010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x11ff value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x11ff] = 0b01010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b01011010, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011010);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x11")
			{
				cpu.y_register(0x11);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x120f value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x120f] = 0b00001111;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b10100101, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b01010101 and "
					"address 0x120f value is 0b01010101")
				{
					cpu.accumulator(0b01010101);
					mem._data[0x120f] = 0b01010101;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x120f value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x120f] = 0b01010101;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b01011010, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011010);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is eor [indirect, x]")
	{
		mem._data[0] = 0x41;

		AND_GIVEN(
			"address value is 0x04, "
			"x register value is 0x04, and "
			"indirect address value is 0x0115")
		{
			mem._data[1] = 0x04;
			mem._data[0x08] = 0x15;
			mem._data[0x09] = 0x01;
			cpu.x_register(0x04);

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x0115 value is 0b00001111")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x0115] = 0b00001111;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"accumulator has a value of 0b10100101, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10100101);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b01010101 and "
				"address 0x0115 value is 0b01010101")
			{
				cpu.accumulator(0b01010101);
				mem._data[0x0115] = 0b01010101;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x0115 value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x0115] = 0b01010101;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"accumulator has a value of 0b01011010, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b01011010);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is eor [indirect, y]")
	{
		mem._data[0] = 0x51;

		AND_GIVEN("address value is 0x08 and indirect address value is 0x01fe")
		{
			mem._data[1] = 0x08;
			mem._data[0x08] = 0xfe;
			mem._data[0x09] = 0x01;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x01ff value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x01ff] = 0b00001111;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b10100101, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b01010101 and "
					"address 0x01ff value is 0b01010101")
				{
					cpu.accumulator(0b01010101);
					mem._data[0x01ff] = 0b01010101;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x01ff value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x01ff] = 0b01010101;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b01011010, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011010);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x02")
			{
				cpu.y_register(0x02);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x0200 value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x0200] = 0b00001111;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0b10100101, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10100101);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b01010101 and "
					"address 0x0200 value is 0b01010101")
				{
					cpu.accumulator(0b01010101);
					mem._data[0x0200] = 0b01010101;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x0200 value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x0200] = 0b01010101;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0b01011010, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011010);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}
}

SCENARIO("Can perform ora operation", "[cpu][logical_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is ora [immediate]")
	{
		mem._data[0] = 0x09;

		AND_GIVEN("accumulator value is 0b10101010 and op value is 0b00001111")
		{
			cpu.accumulator(0b10101010);
			mem._data[1] = 0b00001111;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b10101111, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.accumulator() == 0b10101111);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b00000000 and op value is 0b00000000")
		{
			cpu.accumulator(0b00000000);
			mem._data[1] = 0b00000000;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b00000000, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b00000000);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("accumulator value is 0b00001111 and op value is 0b01010101")
		{
			cpu.accumulator(0b00001111);
			mem._data[1] = 0b01010101;

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"accumulator has a value of 0b01011111, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.accumulator() == 0b01011111);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}
	}

	GIVEN("operation is ora [zero page]")
	{
		mem._data[0] = 0x05;

		AND_GIVEN("address value is 0x27")
		{
			mem._data[1] = 0x27;

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x0027 value is 0b00001111")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x0027] = 0b00001111;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b10101111, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10101111);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00000000 and "
				"address 0x0027 value is 0b00000000")
			{
				cpu.accumulator(0b00000000);
				mem._data[0x0027] = 0b00000000;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x0027 value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x0027] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b01011111, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b01011111);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is ora [zero page, x]")
	{
		mem._data[0] = 0x15;

		AND_GIVEN("address value is 0x27 and x register value is 0x15")
		{
			mem._data[1] = 0x27;
			cpu.x_register(0x15);

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x003c value is 0b00001111")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x003c] = 0b00001111;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b10101111, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10101111);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00000000 and "
				"address 0x003c value is 0b00000000")
			{
				cpu.accumulator(0b00000000);
				mem._data[0x003c] = 0b00000000;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x003c value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x003c] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b01011111, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b01011111);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is ora [absolute]")
	{
		mem._data[0] = 0x0d;

		AND_GIVEN("address value is 0x1527")
		{
			mem._data[2] = 0x15;
			mem._data[1] = 0x27;

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x1527 value is 0b00001111")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x1527] = 0b00001111;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b10101111, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10101111);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00000000 and "
				"address 0x1527 value is 0b00000000")
			{
				cpu.accumulator(0b00000000);
				mem._data[0x1527] = 0b00000000;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x1527 value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x1527] = 0b01010101;

				WHEN("operation is performed (4 cycles)")
				{
					cpu.cycle(4);

					THEN(
						"accumulator has a value of 0b01011111, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b01011111);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is ora [absolute, x]")
	{
		mem._data[0] = 0x1d;

		AND_GIVEN("address value is 0x11fe")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0xfe;

			AND_GIVEN("x register value is 0x01")
			{
				cpu.x_register(0x01);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x11ff value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x11ff] = 0b00001111;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b10101111, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10101111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00000000 and "
					"address 0x11ff value is 0b00000000")
				{
					cpu.accumulator(0b00000000);
					mem._data[0x11ff] = 0b00000000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x11ff value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x11ff] = 0b01010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b01011111, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("x register value is 0x11")
			{
				cpu.x_register(0x11);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x120f value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x120f] = 0b00001111;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b10101111, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10101111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00000000 and "
					"address 0x120f value is 0b00000000")
				{
					cpu.accumulator(0b00000000);
					mem._data[0x120f] = 0b00000000;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x120f value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x120f] = 0b01010101;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b01011111, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is ora [absolute, y]")
	{
		mem._data[0] = 0x19;

		AND_GIVEN("address value is 0x11fe")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0xfe;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x11ff value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x11ff] = 0b00001111;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b10101111, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10101111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00000000 and "
					"address 0x11ff value is 0b00000000")
				{
					cpu.accumulator(0b00000000);
					mem._data[0x11ff] = 0b00000000;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x11ff value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x11ff] = 0b01010101;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0b01011111, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x11")
			{
				cpu.y_register(0x11);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x120f value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x120f] = 0b00001111;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b10101111, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10101111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00000000 and "
					"address 0x120f value is 0b00000000")
				{
					cpu.accumulator(0b00000000);
					mem._data[0x120f] = 0b00000000;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x120f value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x120f] = 0b01010101;

					WHEN("operation is performed with page cross (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b01011111, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is ora [indirect, x]")
	{
		mem._data[0] = 0x01;

		AND_GIVEN(
			"address value is 0x04, "
			"x register value is 0x04, and "
			"indirect address value is 0x0115")
		{
			mem._data[1] = 0x04;
			mem._data[0x08] = 0x15;
			mem._data[0x09] = 0x01;
			cpu.x_register(0x04);

			AND_GIVEN(
				"accumulator value is 0b10101010 and "
				"address 0x0115 value is 0b00001111")
			{
				cpu.accumulator(0b10101010);
				mem._data[0x0115] = 0b00001111;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"accumulator has a value of 0b10101111, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == 0b10101111);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00000000 and "
				"address 0x0115 value is 0b00000000")
			{
				cpu.accumulator(0b00000000);
				mem._data[0x0115] = 0b00000000;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"accumulator has a value of 0b00000000, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b00000000);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN(
				"accumulator value is 0b00001111 and "
				"address 0x0115 value is 0b01010101")
			{
				cpu.accumulator(0b00001111);
				mem._data[0x0115] = 0b01010101;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"accumulator has a value of 0b01011111, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0b01011111);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is ora [indirect, y]")
	{
		mem._data[0] = 0x11;

		AND_GIVEN("address value is 0x08 and indirect address value is 0x01fe")
		{
			mem._data[1] = 0x08;
			mem._data[0x08] = 0xfe;
			mem._data[0x09] = 0x01;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x01ff value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x01ff] = 0b00001111;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b10101111, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10101111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00000000 and "
					"address 0x01ff value is 0b00000000")
				{
					cpu.accumulator(0b00000000);
					mem._data[0x01ff] = 0b00000000;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x01ff value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x01ff] = 0b01010101;

					WHEN("operation is performed (5 cycles)")
					{
						cpu.cycle(5);

						THEN(
							"accumulator has a value of 0b01011111, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x02")
			{
				cpu.y_register(0x02);

				AND_GIVEN(
					"accumulator value is 0b10101010 and "
					"address 0x0200 value is 0b00001111")
				{
					cpu.accumulator(0b10101010);
					mem._data[0x0200] = 0b00001111;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0b10101111, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == 0b10101111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00000000 and "
					"address 0x0200 value is 0b00000000")
				{
					cpu.accumulator(0b00000000);
					mem._data[0x0200] = 0b00000000;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0b00000000, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b00000000);
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 0b00001111 and "
					"address 0x0200 value is 0b01010101")
				{
					cpu.accumulator(0b00001111);
					mem._data[0x0200] = 0b01010101;

					WHEN("operation is performed with page cross (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0b01011111, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0b01011111);
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}
}
