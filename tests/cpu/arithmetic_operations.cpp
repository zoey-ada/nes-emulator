#include <catch2/catch_test_macros.hpp>
#include <cpu/cpu.hpp>
#include <memory/slowMemory.hpp>

SCENARIO("Can perform adc operation", "[cpu][arithmetic_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is adc [immediate]")
	{
		mem._data[0] = 0x69;

		AND_GIVEN("using unsigned numbers")
		{
			AND_GIVEN("accumulator value is 0, op value is 0, and carry is not set")
			{
				cpu.accumulator(0);
				cpu.c_flag(false);
				mem._data[1] = 0;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 0, "
						"c flag is not set, and "
						"z flag is set")
					{
						REQUIRE(cpu.accumulator() == 0);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 15, op value is 23, and carry is not set")
			{
				cpu.accumulator(15);
				cpu.c_flag(false);
				mem._data[1] = 23;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 38, "
						"c flag is not set, and "
						"z flag is not set")
					{
						REQUIRE(cpu.accumulator() == 38);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 15, op value is 23, and carry is set")
			{
				cpu.accumulator(15);
				cpu.c_flag(true);
				mem._data[1] = 23;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 39, "
						"c flag is not set, and "
						"z flag is not set")
					{
						REQUIRE(cpu.accumulator() == 39);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 200, op value is 56, and carry is not set")
			{
				cpu.accumulator(200);
				cpu.c_flag(false);
				mem._data[1] = 56;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 0, "
						"c flag is set, and "
						"z flag is set")
					{
						REQUIRE(cpu.accumulator() == 0);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 200, op value is 60, and carry is not set")
			{
				cpu.accumulator(200);
				cpu.c_flag(false);
				mem._data[1] = 60;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 4, "
						"c flag is set, and "
						"z flag is not set")
					{
						REQUIRE(cpu.accumulator() == 4);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
					}
				}
			}
		}

		AND_GIVEN("using signed numbers")
		{
			AND_GIVEN("accumulator value is 15, op value is -15, and carry is not set")
			{
				cpu.accumulator(15);
				cpu.c_flag(false);
				mem._data[1] = -15;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 0, "
						"c flag is set, "
						"v flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.v_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 20, op value is -15, and carry is not set")
			{
				cpu.accumulator(20);
				cpu.c_flag(false);
				mem._data[1] = -15;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 5, "
						"c flag is set, "
						"v flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 5);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.v_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is -120, op value is -50, and carry is not set")
			{
				cpu.accumulator(-120);
				cpu.c_flag(false);
				mem._data[1] = -50;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 86, "
						"c flag is set, "
						"v flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 86);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.v_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 120, op value is 50, and carry is not set")
			{
				cpu.accumulator(120);
				cpu.c_flag(false);
				mem._data[1] = 50;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of -86, "
						"c flag is not set, "
						"v flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == (uint8_t)-86);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.v_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 10, op value is -20, and carry is set")
			{
				cpu.accumulator(10);
				cpu.c_flag(true);
				mem._data[1] = -20;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of -9, "
						"c flag is not set, "
						"v flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == (uint8_t)-9);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.v_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 127, op value is 127, and carry is not set")
			{
				cpu.accumulator(127);
				cpu.c_flag(false);
				mem._data[1] = 127;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of -9, "
						"c flag is not set, "
						"v flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == (uint8_t)-2);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.v_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is adc [zero page]")
	{
		mem._data[0] = 0x65;

		AND_GIVEN("address value is 0x27")
		{
			mem._data[1] = 0x27;

			AND_GIVEN("using unsigned numbers")
			{
				AND_GIVEN(
					"accumulator value is 0, "
					"address 0x0027 value is 0, and "
					"carry is not set")
				{
					cpu.accumulator(0);
					cpu.c_flag(false);
					mem._data[0x0027] = 0;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 0, "
							"c flag is not set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x0027 value is 23, and "
					"carry is not set")
				{
					cpu.accumulator(15);
					cpu.c_flag(false);
					mem._data[0x0027] = 23;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 38, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 38);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x0027 value is 23, and "
					"carry is set")
				{
					cpu.accumulator(15);
					cpu.c_flag(true);
					mem._data[0x0027] = 23;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 39, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 39);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 200, "
					"address 0x0027 value is 56, and "
					"carry is not set")
				{
					cpu.accumulator(200);
					cpu.c_flag(false);
					mem._data[0x0027] = 56;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 200, "
					"address 0x0027 value is 60, and "
					"carry is not set")
				{
					cpu.accumulator(200);
					cpu.c_flag(false);
					mem._data[0x0027] = 60;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 4, "
							"c flag is set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 4);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}
			}

			AND_GIVEN("using signed numbers")
			{
				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x0027 value is -15, and "
					"carry is not set")
				{
					cpu.accumulator(15);
					cpu.c_flag(false);
					mem._data[0x0027] = -15;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x0027 value is -15, and "
					"carry is not set")
				{
					cpu.accumulator(20);
					cpu.c_flag(false);
					mem._data[0x0027] = -15;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 5, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 5);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -120, "
					"address 0x0027 value is -50, and "
					"carry is not set")
				{
					cpu.accumulator(-120);
					cpu.c_flag(false);
					mem._data[0x0027] = -50;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 86, "
							"c flag is set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 86);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 120, "
					"address 0x0027 value is 50, and "
					"carry is not set")
				{
					cpu.accumulator(120);
					cpu.c_flag(false);
					mem._data[0x0027] = 50;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of -86, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-86);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 10, "
					"address 0x0027 value is -20, and "
					"carry is set")
				{
					cpu.accumulator(10);
					cpu.c_flag(true);
					mem._data[0x0027] = -20;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of -9, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-9);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 127, "
					"address 0x0027 value is 127, and "
					"carry is not set")
				{
					cpu.accumulator(127);
					cpu.c_flag(false);
					mem._data[0x0027] = 127;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of -9, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-2);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is adc [zero page, x]")
	{
		mem._data[0] = 0x75;

		AND_GIVEN("address value is 0x27 and x register value is 0x15")
		{
			mem._data[1] = 0x27;
			cpu.x_register(0x15);

			AND_GIVEN("using unsigned numbers")
			{
				AND_GIVEN(
					"accumulator value is 0, "
					"address 0x003c value is 0, and "
					"carry is not set")
				{
					cpu.accumulator(0);
					cpu.c_flag(false);
					mem._data[0x003c] = 0;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is not set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x003c value is 23, and "
					"carry is not set")
				{
					cpu.accumulator(15);
					cpu.c_flag(false);
					mem._data[0x003c] = 23;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 38, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 38);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x003c value is 23, and "
					"carry is set")
				{
					cpu.accumulator(15);
					cpu.c_flag(true);
					mem._data[0x003c] = 23;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 39, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 39);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 200, "
					"address 0x003c value is 56, and "
					"carry is not set")
				{
					cpu.accumulator(200);
					cpu.c_flag(false);
					mem._data[0x003c] = 56;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 200, "
					"address 0x003c value is 60, and "
					"carry is not set")
				{
					cpu.accumulator(200);
					cpu.c_flag(false);
					mem._data[0x003c] = 60;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 4, "
							"c flag is set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 4);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}
			}

			AND_GIVEN("using signed numbers")
			{
				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x003c value is -15, and "
					"carry is not set")
				{
					cpu.accumulator(15);
					cpu.c_flag(false);
					mem._data[0x003c] = -15;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x003c value is -15, and "
					"carry is not set")
				{
					cpu.accumulator(20);
					cpu.c_flag(false);
					mem._data[0x003c] = -15;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 5, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 5);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -120, "
					"address 0x003c value is -50, and "
					"carry is not set")
				{
					cpu.accumulator(-120);
					cpu.c_flag(false);
					mem._data[0x003c] = -50;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 86, "
							"c flag is set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 86);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 120, "
					"address 0x003c value is 50, and "
					"carry is not set")
				{
					cpu.accumulator(120);
					cpu.c_flag(false);
					mem._data[0x003c] = 50;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -86, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-86);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 10, "
					"address 0x003c value is -20, and "
					"carry is set")
				{
					cpu.accumulator(10);
					cpu.c_flag(true);
					mem._data[0x003c] = -20;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -9, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-9);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 127, "
					"address 0x003c value is 127, and "
					"carry is not set")
				{
					cpu.accumulator(127);
					cpu.c_flag(false);
					mem._data[0x003c] = 127;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -9, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-2);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is adc [absolute]")
	{
		mem._data[0] = 0x6d;

		AND_GIVEN("address value is 0x1527")
		{
			mem._data[2] = 0x15;
			mem._data[1] = 0x27;

			AND_GIVEN("using unsigned numbers")
			{
				AND_GIVEN(
					"accumulator value is 0, "
					"address 0x1527 value is 0, and "
					"carry is not set")
				{
					cpu.accumulator(0);
					cpu.c_flag(false);
					mem._data[0x1527] = 0;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is not set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x1527 value is 23, and "
					"carry is not set")
				{
					cpu.accumulator(15);
					cpu.c_flag(false);
					mem._data[0x1527] = 23;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 38, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 38);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x1527 value is 23, and "
					"carry is set")
				{
					cpu.accumulator(15);
					cpu.c_flag(true);
					mem._data[0x1527] = 23;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 39, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 39);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 200, "
					"address 0x1527 value is 56, and "
					"carry is not set")
				{
					cpu.accumulator(200);
					cpu.c_flag(false);
					mem._data[0x1527] = 56;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 200, "
					"address 0x1527 value is 60, and "
					"carry is not set")
				{
					cpu.accumulator(200);
					cpu.c_flag(false);
					mem._data[0x1527] = 60;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 4, "
							"c flag is set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 4);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}
			}

			AND_GIVEN("using signed numbers")
			{
				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x1527 value is -15, and "
					"carry is not set")
				{
					cpu.accumulator(15);
					cpu.c_flag(false);
					mem._data[0x1527] = -15;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x1527 value is -15, and "
					"carry is not set")
				{
					cpu.accumulator(20);
					cpu.c_flag(false);
					mem._data[0x1527] = -15;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 5, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 5);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -120, "
					"address 0x1527 value is -50, and "
					"carry is not set")
				{
					cpu.accumulator(-120);
					cpu.c_flag(false);
					mem._data[0x1527] = -50;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 86, "
							"c flag is set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 86);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 120, "
					"address 0x1527 value is 50, and "
					"carry is not set")
				{
					cpu.accumulator(120);
					cpu.c_flag(false);
					mem._data[0x1527] = 50;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -86, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-86);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 10, "
					"address 0x1527 value is -20, and "
					"carry is set")
				{
					cpu.accumulator(10);
					cpu.c_flag(true);
					mem._data[0x1527] = -20;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -9, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-9);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 127, "
					"address 0x1527 value is 127, and "
					"carry is not set")
				{
					cpu.accumulator(127);
					cpu.c_flag(false);
					mem._data[0x1527] = 127;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -9, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-2);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is adc [absolute, x]")
	{
		mem._data[0] = 0x7d;

		AND_GIVEN("address value is 0x11fe")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0xfe;

			AND_GIVEN("x register value is 0x01")
			{
				cpu.x_register(0x01);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x11ff value is 0, and "
						"carry is not set")
					{
						cpu.accumulator(0);
						cpu.c_flag(false);
						mem._data[0x11ff] = 0;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is not set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x11ff value is 23, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x11ff] = 23;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 38, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 38);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x11ff value is 23, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x11ff] = 23;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 39, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 39);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x11ff value is 56, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x11ff] = 56;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x11ff value is 60, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x11ff] = 60;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 4, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 4);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x11ff value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x11ff] = -15;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x11ff value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x11ff] = -15;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 5, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 5);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -120, "
						"address 0x11ff value is -50, and "
						"carry is not set")
					{
						cpu.accumulator(-120);
						cpu.c_flag(false);
						mem._data[0x11ff] = -50;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 86, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 86);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 120, "
						"address 0x11ff value is 50, and "
						"carry is not set")
					{
						cpu.accumulator(120);
						cpu.c_flag(false);
						mem._data[0x11ff] = 50;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -86, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-86);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 10, "
						"address 0x11ff value is -20, and "
						"carry is set")
					{
						cpu.accumulator(10);
						cpu.c_flag(true);
						mem._data[0x11ff] = -20;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-9);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x11ff value is 127, and "
						"carry is not set")
					{
						cpu.accumulator(127);
						cpu.c_flag(false);
						mem._data[0x11ff] = 127;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}
				}
			}

			AND_GIVEN("x register value is 0x11")
			{
				cpu.x_register(0x11);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x120f value is 0, and "
						"carry is not set")
					{
						cpu.accumulator(0);
						cpu.c_flag(false);
						mem._data[0x120f] = 0;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is not set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x120f value is 23, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x120f] = 23;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 38, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 38);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x120f value is 23, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x120f] = 23;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 39, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 39);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x120f value is 56, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x120f] = 56;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x120f value is 60, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x120f] = 60;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 4, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 4);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x120f value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x120f] = -15;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x120f value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x120f] = -15;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 5, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 5);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -120, "
						"address 0x120f value is -50, and "
						"carry is not set")
					{
						cpu.accumulator(-120);
						cpu.c_flag(false);
						mem._data[0x120f] = -50;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 86, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 86);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 120, "
						"address 0x120f value is 50, and "
						"carry is not set")
					{
						cpu.accumulator(120);
						cpu.c_flag(false);
						mem._data[0x120f] = 50;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -86, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-86);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 10, "
						"address 0x120f value is -20, and "
						"carry is set")
					{
						cpu.accumulator(10);
						cpu.c_flag(true);
						mem._data[0x120f] = -20;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-9);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x120f value is 127, and "
						"carry is not set")
					{
						cpu.accumulator(127);
						cpu.c_flag(false);
						mem._data[0x120f] = 127;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}
				}
			}
		}
	}

	GIVEN("operation is adc [absolute, y]")
	{
		mem._data[0] = 0x79;

		AND_GIVEN("address value is 0x11fe")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0xfe;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x11ff value is 0, and "
						"carry is not set")
					{
						cpu.accumulator(0);
						cpu.c_flag(false);
						mem._data[0x11ff] = 0;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is not set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x11ff value is 23, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x11ff] = 23;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 38, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 38);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x11ff value is 23, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x11ff] = 23;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 39, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 39);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x11ff value is 56, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x11ff] = 56;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x11ff value is 60, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x11ff] = 60;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 4, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 4);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x11ff value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x11ff] = -15;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x11ff value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x11ff] = -15;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 5, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 5);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -120, "
						"address 0x11ff value is -50, and "
						"carry is not set")
					{
						cpu.accumulator(-120);
						cpu.c_flag(false);
						mem._data[0x11ff] = -50;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 86, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 86);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 120, "
						"address 0x11ff value is 50, and "
						"carry is not set")
					{
						cpu.accumulator(120);
						cpu.c_flag(false);
						mem._data[0x11ff] = 50;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -86, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-86);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 10, "
						"address 0x11ff value is -20, and "
						"carry is set")
					{
						cpu.accumulator(10);
						cpu.c_flag(true);
						mem._data[0x11ff] = -20;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-9);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x11ff value is 127, and "
						"carry is not set")
					{
						cpu.accumulator(127);
						cpu.c_flag(false);
						mem._data[0x11ff] = 127;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x11")
			{
				cpu.y_register(0x11);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x120f value is 0, and "
						"carry is not set")
					{
						cpu.accumulator(0);
						cpu.c_flag(false);
						mem._data[0x120f] = 0;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is not set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x120f value is 23, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x120f] = 23;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 38, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 38);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x120f value is 23, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x120f] = 23;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 39, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 39);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x120f value is 56, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x120f] = 56;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x120f value is 60, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x120f] = 60;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 4, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 4);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x120f value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x120f] = -15;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x120f value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x120f] = -15;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 5, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 5);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -120, "
						"address 0x120f value is -50, and "
						"carry is not set")
					{
						cpu.accumulator(-120);
						cpu.c_flag(false);
						mem._data[0x120f] = -50;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 86, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 86);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 120, "
						"address 0x120f value is 50, and "
						"carry is not set")
					{
						cpu.accumulator(120);
						cpu.c_flag(false);
						mem._data[0x120f] = 50;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -86, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-86);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 10, "
						"address 0x120f value is -20, and "
						"carry is set")
					{
						cpu.accumulator(10);
						cpu.c_flag(true);
						mem._data[0x120f] = -20;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-9);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x120f value is 127, and "
						"carry is not set")
					{
						cpu.accumulator(127);
						cpu.c_flag(false);
						mem._data[0x120f] = 127;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}
				}
			}
		}
	}

	GIVEN("operation is adc [indirect, x]")
	{
		mem._data[0] = 0x61;

		AND_GIVEN(
			"address value is 0x04, "
			"x register value is 0x04, and "
			"indirect address value is 0x0115")
		{
			mem._data[1] = 0x04;
			mem._data[0x08] = 0x15;
			mem._data[0x09] = 0x01;
			cpu.x_register(0x04);

			AND_GIVEN("using unsigned numbers")
			{
				AND_GIVEN(
					"accumulator value is 0, "
					"address 0x0115 value is 0, and "
					"carry is not set")
				{
					cpu.accumulator(0);
					cpu.c_flag(false);
					mem._data[0x0115] = 0;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0, "
							"c flag is not set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x0115 value is 23, and "
					"carry is not set")
				{
					cpu.accumulator(15);
					cpu.c_flag(false);
					mem._data[0x0115] = 23;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 38, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 38);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x0115 value is 23, and "
					"carry is set")
				{
					cpu.accumulator(15);
					cpu.c_flag(true);
					mem._data[0x0115] = 23;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 39, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 39);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 200, "
					"address 0x0115 value is 56, and "
					"carry is not set")
				{
					cpu.accumulator(200);
					cpu.c_flag(false);
					mem._data[0x0115] = 56;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 200, "
					"address 0x0115 value is 60, and "
					"carry is not set")
				{
					cpu.accumulator(200);
					cpu.c_flag(false);
					mem._data[0x0115] = 60;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 4, "
							"c flag is set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 4);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}
			}

			AND_GIVEN("using signed numbers")
			{
				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x0115 value is -15, and "
					"carry is not set")
				{
					cpu.accumulator(15);
					cpu.c_flag(false);
					mem._data[0x0115] = -15;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x0115 value is -15, and "
					"carry is not set")
				{
					cpu.accumulator(20);
					cpu.c_flag(false);
					mem._data[0x0115] = -15;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 5, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 5);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -120, "
					"address 0x0115 value is -50, and "
					"carry is not set")
				{
					cpu.accumulator(-120);
					cpu.c_flag(false);
					mem._data[0x0115] = -50;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 86, "
							"c flag is set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 86);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 120, "
					"address 0x0115 value is 50, and "
					"carry is not set")
				{
					cpu.accumulator(120);
					cpu.c_flag(false);
					mem._data[0x0115] = 50;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of -86, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-86);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 10, "
					"address 0x0115 value is -20, and "
					"carry is set")
				{
					cpu.accumulator(10);
					cpu.c_flag(true);
					mem._data[0x0115] = -20;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of -9, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-9);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 127, "
					"address 0x0115 value is 127, and "
					"carry is not set")
				{
					cpu.accumulator(127);
					cpu.c_flag(false);
					mem._data[0x0115] = 127;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of -9, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-2);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is adc [indirect, y]")
	{
		mem._data[0] = 0x71;

		AND_GIVEN("address value is 0x08 and indirect address value is 0x01fe")
		{
			mem._data[1] = 0x08;
			mem._data[0x08] = 0xfe;
			mem._data[0x09] = 0x01;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x01ff value is 0, and "
						"carry is not set")
					{
						cpu.accumulator(0);
						cpu.c_flag(false);
						mem._data[0x01ff] = 0;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is not set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x01ff value is 23, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x01ff] = 23;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 38, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 38);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x01ff value is 23, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x01ff] = 23;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 39, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 39);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x01ff value is 56, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x01ff] = 56;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x01ff value is 60, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x01ff] = 60;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 4, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 4);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x01ff value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x01ff] = -15;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x01ff value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x01ff] = -15;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 5, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 5);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -120, "
						"address 0x01ff value is -50, and "
						"carry is not set")
					{
						cpu.accumulator(-120);
						cpu.c_flag(false);
						mem._data[0x01ff] = -50;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 86, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 86);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 120, "
						"address 0x01ff value is 50, and "
						"carry is not set")
					{
						cpu.accumulator(120);
						cpu.c_flag(false);
						mem._data[0x01ff] = 50;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -86, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-86);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 10, "
						"address 0x01ff value is -20, and "
						"carry is set")
					{
						cpu.accumulator(10);
						cpu.c_flag(true);
						mem._data[0x01ff] = -20;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-9);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x01ff value is 127, and "
						"carry is not set")
					{
						cpu.accumulator(127);
						cpu.c_flag(false);
						mem._data[0x01ff] = 127;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x02")
			{
				cpu.y_register(0x02);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x0200 value is 0, and "
						"carry is not set")
					{
						cpu.accumulator(0);
						cpu.c_flag(false);
						mem._data[0x0200] = 0;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 0, "
								"c flag is not set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x0200 value is 23, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x0200] = 23;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 38, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 38);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x0200 value is 23, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x0200] = 23;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 39, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 39);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x0200 value is 56, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x0200] = 56;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 200, "
						"address 0x0200 value is 60, and "
						"carry is not set")
					{
						cpu.accumulator(200);
						cpu.c_flag(false);
						mem._data[0x0200] = 60;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 4, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 4);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x0200 value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(15);
						cpu.c_flag(false);
						mem._data[0x0200] = -15;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x0200 value is -15, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x0200] = -15;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 5, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 5);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -120, "
						"address 0x0200 value is -50, and "
						"carry is not set")
					{
						cpu.accumulator(-120);
						cpu.c_flag(false);
						mem._data[0x0200] = -50;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 86, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 86);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 120, "
						"address 0x0200 value is 50, and "
						"carry is not set")
					{
						cpu.accumulator(120);
						cpu.c_flag(false);
						mem._data[0x0200] = 50;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of -86, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-86);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 10, "
						"address 0x0200 value is -20, and "
						"carry is set")
					{
						cpu.accumulator(10);
						cpu.c_flag(true);
						mem._data[0x0200] = -20;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-9);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x0200 value is 127, and "
						"carry is not set")
					{
						cpu.accumulator(127);
						cpu.c_flag(false);
						mem._data[0x0200] = 127;

						WHEN("operation is performed with page cross (6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of -9, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}
				}
			}
		}
	}
}

SCENARIO("Can perform dec operation", "[cpu][arithmetic_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is dec [zero page]")
	{
		mem._data[0] = 0xc6;

		AND_GIVEN("address value is 0x26")
		{
			mem._data[1] = 0x26;

			AND_GIVEN("address 0x0026 value is 1")
			{
				mem._data[0x0026] = 1;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0026 has a value of 0, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0026] == 0);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0026 value is 8")
			{
				mem._data[0x0026] = 8;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0026 has a value of 7, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0026] == 7);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0026 value is -21")
			{
				mem._data[0x0026] = -21;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0026 has a value of -22, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0026] == (uint8_t)-22);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is dec [zero page, x]")
	{
		mem._data[0] = 0xd6;

		AND_GIVEN("address value is 0x13 and x register value is 0x0d")
		{
			mem._data[1] = 0x13;
			cpu.x_register(0x0d);

			AND_GIVEN("address 0x0020 value is 1")
			{
				mem._data[0x0020] = 1;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0020 has a value of 0, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0020] == 0);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0020 value is 8")
			{
				mem._data[0x0020] = 8;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0020 has a value of 7, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0020] == 7);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0020 value is -21")
			{
				mem._data[0x0020] = -21;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0020 has a value of -22, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0020] == (uint8_t)-22);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is dec [absolute]")
	{
		mem._data[0] = 0xce;

		AND_GIVEN("address value is 0x2631")
		{
			mem._data[2] = 0x26;
			mem._data[1] = 0x31;

			AND_GIVEN("address 0x2631 value is 1")
			{
				mem._data[0x2631] = 1;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x2631 has a value of 0, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x2631] == 0);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2631 value is 8")
			{
				mem._data[0x2631] = 8;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x2631 has a value of 7, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x2631] == 7);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2631 value is -21")
			{
				mem._data[0x2631] = -21;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x2631 has a value of -22, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x2631] == (uint8_t)-22);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is dec [absolute, x]")
	{
		mem._data[0] = 0xde;

		AND_GIVEN("address value is 0x2629 and x register value is 0x08")
		{
			mem._data[2] = 0x26;
			mem._data[1] = 0x29;
			cpu.x_register(0x08);

			AND_GIVEN("address 0x2631 value is 1")
			{
				mem._data[0x2631] = 1;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x2631 has a value of 0, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x2631] == 0);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2631 value is 8")
			{
				mem._data[0x2631] = 8;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x2631 has a value of 7, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x2631] == 7);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2631 value is -21")
			{
				mem._data[0x2631] = -21;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x2631 has a value of -22, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x2631] == (uint8_t)-22);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}
}

SCENARIO("Can perform dex operation", "[cpu][arithmetic_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is dex [implicit]")
	{
		mem._data[0] = 0xca;

		AND_GIVEN("x register value is 1")
		{
			cpu.x_register(1);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"x register has a value of 0, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.x_register() == 0);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("x register value is 8")
		{
			cpu.x_register(8);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"x register has a value of 7, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.x_register() == 7);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("x register value is -21")
		{
			cpu.x_register(-21);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"x register has a value of -22, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.x_register() == (uint8_t)-22);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}
}

SCENARIO("Can perform dey operation", "[cpu][arithmetic_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is dey [implicit]")
	{
		mem._data[0] = 0x88;

		AND_GIVEN("y register value is 1")
		{
			cpu.y_register(1);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"y register has a value of 0, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.y_register() == 0);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("y register value is 8")
		{
			cpu.y_register(8);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"y register has a value of 7, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.y_register() == 7);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("y register value is -21")
		{
			cpu.y_register(-21);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"y register has a value of -22, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.y_register() == (uint8_t)-22);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}
}

SCENARIO("Can perform inc operation", "[cpu][arithmetic_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is inc [zero page]")
	{
		mem._data[0] = 0xe6;

		AND_GIVEN("address value is 0x26")
		{
			mem._data[1] = 0x26;

			AND_GIVEN("address 0x0026 value is -1")
			{
				mem._data[0x0026] = -1;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0026 has a value of 0, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0026] == 0);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0026 value is 6")
			{
				mem._data[0x0026] = 6;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0026 has a value of 7, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0026] == 7);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0026 value is -23")
			{
				mem._data[0x0026] = -23;

				WHEN("operation is performed (5 cycles)")
				{
					cpu.cycle(5);

					THEN(
						"address 0x0026 has a value of -22, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0026] == (uint8_t)-22);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is inc [zero page, x]")
	{
		mem._data[0] = 0xf6;

		AND_GIVEN("address value is 0x13 and x register value is 0x0d")
		{
			mem._data[1] = 0x13;
			cpu.x_register(0x0d);

			AND_GIVEN("address 0x0020 value is -1")
			{
				mem._data[0x0020] = -1;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0020 has a value of 0, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0020] == 0);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0020 value is 6")
			{
				mem._data[0x0020] = 6;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0020 has a value of 7, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x0020] == 7);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x0020 value is -23")
			{
				mem._data[0x0020] = -23;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x0020 has a value of -22, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x0020] == (uint8_t)-22);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is inc [absolute]")
	{
		mem._data[0] = 0xee;

		AND_GIVEN("address value is 0x2631")
		{
			mem._data[2] = 0x26;
			mem._data[1] = 0x31;

			AND_GIVEN("address 0x2631 value is -1")
			{
				mem._data[0x2631] = -1;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x2631 has a value of 0, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x2631] == 0);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2631 value is 6")
			{
				mem._data[0x2631] = 6;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x2631 has a value of 7, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x2631] == 7);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2631 value is -23")
			{
				mem._data[0x2631] = -23;

				WHEN("operation is performed (6 cycles)")
				{
					cpu.cycle(6);

					THEN(
						"address 0x2631 has a value of -22, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x2631] == (uint8_t)-22);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is inc [absolute, x]")
	{
		mem._data[0] = 0xfe;

		AND_GIVEN("address value is 0x2629 and x register value is 0x08")
		{
			mem._data[2] = 0x26;
			mem._data[1] = 0x29;
			cpu.x_register(0x08);

			AND_GIVEN("address 0x2631 value is -1")
			{
				mem._data[0x2631] = -1;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x2631 has a value of 0, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x2631] == 0);
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2631 value is 6")
			{
				mem._data[0x2631] = 6;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x2631 has a value of 7, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(mem._data[0x2631] == 7);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("address 0x2631 value is -23")
			{
				mem._data[0x2631] = -23;

				WHEN("operation is performed (7 cycles)")
				{
					cpu.cycle(7);

					THEN(
						"address 0x2631 has a value of -22, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(mem._data[0x2631] == (uint8_t)-22);
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}
		}
	}
}

SCENARIO("Can perform inx operation", "[cpu][arithmetic_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is inx [implicit]")
	{
		mem._data[0] = 0xe8;

		AND_GIVEN("x register value is -1")
		{
			cpu.x_register(-1);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"x register has a value of 0, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.x_register() == 0);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("x register value is 6")
		{
			cpu.x_register(6);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"x register has a value of 7, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.x_register() == 7);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("x register value is -23")
		{
			cpu.x_register(-23);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"x register has a value of -22, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.x_register() == (uint8_t)-22);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}
}

SCENARIO("Can perform iny operation", "[cpu][arithmetic_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is iny [implicit]")
	{
		mem._data[0] = 0xc8;

		AND_GIVEN("y register value is -1")
		{
			cpu.y_register(-1);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"y register has a value of 0, "
					"z flag is set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.y_register() == 0);
					REQUIRE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("y register value is 6")
		{
			cpu.y_register(6);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"y register has a value of 7, "
					"z flag is not set, and "
					"n flag is not set")
				{
					REQUIRE(cpu.y_register() == 7);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE_FALSE(cpu.n_flag());
				}
			}
		}

		AND_GIVEN("y register value is -23")
		{
			cpu.y_register(-23);

			WHEN("operation is performed (2 cycles)")
			{
				cpu.cycle(2);

				THEN(
					"y register has a value of -22, "
					"z flag is not set, and "
					"n flag is set")
				{
					REQUIRE(cpu.y_register() == (uint8_t)-22);
					REQUIRE_FALSE(cpu.z_flag());
					REQUIRE(cpu.n_flag());
				}
			}
		}
	}
}

SCENARIO("Can perform sbc operation", "[cpu][arithmetic_ops]")
{
	SlowMemory mem;
	Cpu cpu(&mem);
	cpu.program_counter(0x0000);

	GIVEN("operation is sbc [immediate]")
	{
		mem._data[0] = 0xe9;

		AND_GIVEN("using unsigned numbers")
		{
			AND_GIVEN("accumulator value is 0, op value is 0, and carry is set")
			{
				cpu.accumulator(0);
				cpu.c_flag(true);
				mem._data[1] = 0;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 0, "
						"c flag is set, and "
						"z flag is set")
					{
						REQUIRE(cpu.accumulator() == 0);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 20, op value is 10, and carry is set")
			{
				cpu.accumulator(20);
				cpu.c_flag(true);
				mem._data[1] = 10;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 10, "
						"c flag is set, and "
						"z flag is not set")
					{
						REQUIRE(cpu.accumulator() == 10);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 20, op value is 20, and carry is set")
			{
				cpu.accumulator(20);
				cpu.c_flag(true);
				mem._data[1] = 20;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 0, "
						"c flag is set, and "
						"z flag is set")
					{
						REQUIRE(cpu.accumulator() == 0);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.z_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 20, op value is 20, and carry is not set")
			{
				cpu.accumulator(20);
				cpu.c_flag(false);
				mem._data[1] = 20;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of -1, "
						"c flag is not set, and "
						"z flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0xff);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 20, op value is 30, and carry is set")
			{
				cpu.accumulator(20);
				cpu.c_flag(true);
				mem._data[1] = 30;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of -10, "
						"c flag is not set, and "
						"z flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0xf6);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.z_flag());
					}
				}
			}
		}

		AND_GIVEN("using signed numbers")
		{
			AND_GIVEN("accumulator value is -15, op value is -15, and carry is set")
			{
				cpu.accumulator(-15);
				cpu.c_flag(true);
				mem._data[1] = -15;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 0, "
						"c flag is set, "
						"v flag is not set, "
						"z flag is set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 0);
						REQUIRE(cpu.c_flag());
						REQUIRE_FALSE(cpu.v_flag());
						REQUIRE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is -15, op value is -10, and carry is set")
			{
				cpu.accumulator(-15);
				cpu.c_flag(true);
				mem._data[1] = -10;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of -5, "
						"c flag is not set, "
						"v flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == (uint8_t)-5);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.v_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is -15, op value is -10, and carry is not set")
			{
				cpu.accumulator(-15);
				cpu.c_flag(false);
				mem._data[1] = -10;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of -6, "
						"c flag is not set, "
						"v flag is not set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == (uint8_t)-6);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.v_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 15, op value is -10, and carry is set")
			{
				cpu.accumulator(15);
				cpu.c_flag(true);
				mem._data[1] = -10;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 25, "
						"c flag is not set, "
						"v flag is not set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 25);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE_FALSE(cpu.v_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is 127, op value is -127, and carry is set")
			{
				cpu.accumulator(127);
				cpu.c_flag(true);
				mem._data[1] = -127;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of -2, "
						"c flag is not set, "
						"v flag is set, "
						"z flag is not set, and "
						"n flag is set")
					{
						REQUIRE(cpu.accumulator() == (uint8_t)-2);
						REQUIRE_FALSE(cpu.c_flag());
						REQUIRE(cpu.v_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE(cpu.n_flag());
					}
				}
			}

			AND_GIVEN("accumulator value is -127, op value is 127, and carry is set")
			{
				cpu.accumulator(-127);
				cpu.c_flag(true);
				mem._data[1] = 127;

				WHEN("operation is performed (2 cycles)")
				{
					cpu.cycle(2);

					THEN(
						"accumulator has a value of 2, "
						"c flag is set, "
						"v flag is set, "
						"z flag is not set, and "
						"n flag is not set")
					{
						REQUIRE(cpu.accumulator() == 2);
						REQUIRE(cpu.c_flag());
						REQUIRE(cpu.v_flag());
						REQUIRE_FALSE(cpu.z_flag());
						REQUIRE_FALSE(cpu.n_flag());
					}
				}
			}
		}
	}

	GIVEN("operation is sbc [zero page]")
	{
		mem._data[0] = 0xe5;

		AND_GIVEN("address value is 0x27")
		{
			mem._data[1] = 0x27;

			AND_GIVEN("using unsigned numbers")
			{
				AND_GIVEN(
					"accumulator value is 0, "
					"address 0x0027 value is 0, and "
					"carry is set")
				{
					cpu.accumulator(0);
					cpu.c_flag(true);
					mem._data[0x0027] = 0;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x0027 value is 10, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x0027] = 10;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 10, "
							"c flag is set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 10);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x0027 value is 20, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x0027] = 20;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x0027 value is 20, and "
					"carry is not set")
				{
					cpu.accumulator(20);
					cpu.c_flag(false);
					mem._data[0x0027] = 20;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 255, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 255);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x0027 value is 30, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x0027] = 30;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 246, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 246);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}
			}

			AND_GIVEN("using signed numbers")
			{
				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x0027 value is -15, and "
					"carry is set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(true);
					mem._data[0x0027] = -15;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x0027 value is -10, and "
					"carry is set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(true);
					mem._data[0x0027] = -10;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of -5, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-5);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x0027 value is -10, and "
					"carry is not set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(false);
					mem._data[0x0027] = -10;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of -6, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-6);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x0027 value is -10, and "
					"carry is set")
				{
					cpu.accumulator(15);
					cpu.c_flag(true);
					mem._data[0x0027] = -10;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 25, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 25);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 127, "
					"address 0x0027 value is -127, and "
					"carry is set")
				{
					cpu.accumulator(127);
					cpu.c_flag(true);
					mem._data[0x0027] = -127;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of -2, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-2);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -127, "
					"address 0x0027 value is 127, and "
					"carry is set")
				{
					cpu.accumulator(-127);
					cpu.c_flag(true);
					mem._data[0x0027] = 127;

					WHEN("operation is performed (3 cycles)")
					{
						cpu.cycle(3);

						THEN(
							"accumulator has a value of 2, "
							"c flag is set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 2);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is sbc [zero page, x]")
	{
		mem._data[0] = 0xf5;

		AND_GIVEN("address value is 0x27 and x register value is 0x15")
		{
			mem._data[1] = 0x27;
			cpu.x_register(0x15);

			AND_GIVEN("using unsigned numbers")
			{
				AND_GIVEN(
					"accumulator value is 0, "
					"address 0x003c value is 0, and "
					"carry is set")
				{
					cpu.accumulator(0);
					cpu.c_flag(true);
					mem._data[0x003c] = 0;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x003c value is 10, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x003c] = 10;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 10, "
							"c flag is set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 10);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x003c value is 20, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x003c] = 20;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x003c value is 20, and "
					"carry is not set")
				{
					cpu.accumulator(20);
					cpu.c_flag(false);
					mem._data[0x003c] = 20;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 255, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 255);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x003c value is 30, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x003c] = 30;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 246, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 246);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}
			}

			AND_GIVEN("using signed numbers")
			{
				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x003c value is -15, and "
					"carry is set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(true);
					mem._data[0x003c] = -15;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x003c value is -10, and "
					"carry is set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(true);
					mem._data[0x003c] = -10;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -5, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-5);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x003c value is -10, and "
					"carry is not set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(false);
					mem._data[0x003c] = -10;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -6, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-6);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x003c value is -10, and "
					"carry is set")
				{
					cpu.accumulator(15);
					cpu.c_flag(true);
					mem._data[0x003c] = -10;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 25, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 25);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 127, "
					"address 0x003c value is -127, and "
					"carry is set")
				{
					cpu.accumulator(127);
					cpu.c_flag(true);
					mem._data[0x003c] = -127;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -2, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-2);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -127, "
					"address 0x003c value is 127, and "
					"carry is set")
				{
					cpu.accumulator(-127);
					cpu.c_flag(true);
					mem._data[0x003c] = 127;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 2, "
							"c flag is set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 2);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is sbc [absolute]")
	{
		mem._data[0] = 0xed;

		AND_GIVEN("address value is 0x1527")
		{
			mem._data[2] = 0x15;
			mem._data[1] = 0x27;

			AND_GIVEN("using unsigned numbers")
			{
				AND_GIVEN(
					"accumulator value is 0, "
					"address 0x1527 value is 0, and "
					"carry is set")
				{
					cpu.accumulator(0);
					cpu.c_flag(true);
					mem._data[0x1527] = 0;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x1527 value is 10, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x1527] = 10;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 10, "
							"c flag is set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 10);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x1527 value is 20, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x1527] = 20;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x1527 value is 20, and "
					"carry is not set")
				{
					cpu.accumulator(20);
					cpu.c_flag(false);
					mem._data[0x1527] = 20;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 255, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 255);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x1527 value is 30, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x1527] = 30;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 246, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 246);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}
			}

			AND_GIVEN("using signed numbers")
			{
				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x1527 value is -15, and "
					"carry is set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(true);
					mem._data[0x1527] = -15;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x1527 value is -10, and "
					"carry is set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(true);
					mem._data[0x1527] = -10;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -5, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-5);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x1527 value is -10, and "
					"carry is not set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(false);
					mem._data[0x1527] = -10;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -6, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-6);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x1527 value is -10, and "
					"carry is set")
				{
					cpu.accumulator(15);
					cpu.c_flag(true);
					mem._data[0x1527] = -10;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 25, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 25);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 127, "
					"address 0x1527 value is -127, and "
					"carry is set")
				{
					cpu.accumulator(127);
					cpu.c_flag(true);
					mem._data[0x1527] = -127;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of -2, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-2);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -127, "
					"address 0x1527 value is 127, and "
					"carry is set")
				{
					cpu.accumulator(-127);
					cpu.c_flag(true);
					mem._data[0x1527] = 127;

					WHEN("operation is performed (4 cycles)")
					{
						cpu.cycle(4);

						THEN(
							"accumulator has a value of 2, "
							"c flag is set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 2);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is sbc [absolute, x]")
	{
		mem._data[0] = 0xfd;

		AND_GIVEN("address value is 0x11fe")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0xfe;

			AND_GIVEN("x register value is 0x01")
			{
				cpu.x_register(0x01);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x11ff value is 0, and "
						"carry is set")
					{
						cpu.accumulator(0);
						cpu.c_flag(true);
						mem._data[0x11ff] = 0;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x11ff value is 10, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x11ff] = 10;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 10, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 10);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x11ff value is 20, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x11ff] = 20;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x11ff value is 20, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x11ff] = 20;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 255, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 255);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x11ff value is 30, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x11ff] = 30;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 246, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 246);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x11ff value is -15, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x11ff] = -15;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x11ff value is -10, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x11ff] = -10;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -5, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-5);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x11ff value is -10, and "
						"carry is not set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(false);
						mem._data[0x11ff] = -10;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -6, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-6);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x11ff value is -10, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x11ff] = -10;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 25, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 25);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x11ff value is -127, and "
						"carry is set")
					{
						cpu.accumulator(127);
						cpu.c_flag(true);
						mem._data[0x11ff] = -127;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -2, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -127, "
						"address 0x11ff value is 127, and "
						"carry is set")
					{
						cpu.accumulator(-127);
						cpu.c_flag(true);
						mem._data[0x11ff] = 127;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 2, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 2);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}
				}
			}

			AND_GIVEN("x register value is 0x11")
			{
				cpu.x_register(0x11);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x120f value is 0, and "
						"carry is set")
					{
						cpu.accumulator(0);
						cpu.c_flag(true);
						mem._data[0x120f] = 0;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x120f value is 10, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x120f] = 10;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 10, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 10);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x120f value is 20, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x120f] = 20;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x120f value is 20, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x120f] = 20;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 255, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 255);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x120f value is 30, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x120f] = 30;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 246, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 246);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x120f value is -15, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x120f] = -15;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x120f value is -10, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x120f] = -10;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -5, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-5);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x120f value is -10, and "
						"carry is not set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(false);
						mem._data[0x120f] = -10;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -6, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-6);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x120f value is -10, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x120f] = -10;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 25, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 25);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x120f value is -127, and "
						"carry is set")
					{
						cpu.accumulator(127);
						cpu.c_flag(true);
						mem._data[0x120f] = -127;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -2, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -127, "
						"address 0x120f value is 127, and "
						"carry is set")
					{
						cpu.accumulator(-127);
						cpu.c_flag(true);
						mem._data[0x120f] = 127;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 2, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 2);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}
				}
			}
		}
	}

	GIVEN("operation is sbc [absolute, y]")
	{
		mem._data[0] = 0xf9;

		AND_GIVEN("address value is 0x11fe")
		{
			mem._data[2] = 0x11;
			mem._data[1] = 0xfe;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x11ff value is 0, and "
						"carry is set")
					{
						cpu.accumulator(0);
						cpu.c_flag(true);
						mem._data[0x11ff] = 0;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x11ff value is 10, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x11ff] = 10;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 10, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 10);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x11ff value is 20, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x11ff] = 20;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x11ff value is 20, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x11ff] = 20;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 255, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 255);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x11ff value is 30, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x11ff] = 30;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 246, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 246);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x11ff value is -15, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x11ff] = -15;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x11ff value is -10, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x11ff] = -10;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -5, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-5);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x11ff value is -10, and "
						"carry is not set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(false);
						mem._data[0x11ff] = -10;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -6, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-6);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x11ff value is -10, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x11ff] = -10;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 25, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 25);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x11ff value is -127, and "
						"carry is set")
					{
						cpu.accumulator(127);
						cpu.c_flag(true);
						mem._data[0x11ff] = -127;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of -2, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -127, "
						"address 0x11ff value is 127, and "
						"carry is set")
					{
						cpu.accumulator(-127);
						cpu.c_flag(true);
						mem._data[0x11ff] = 127;

						WHEN("operation is performed (4 cycles)")
						{
							cpu.cycle(4);

							THEN(
								"accumulator has a value of 2, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 2);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x11")
			{
				cpu.y_register(0x11);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x120f value is 0, and "
						"carry is set")
					{
						cpu.accumulator(0);
						cpu.c_flag(true);
						mem._data[0x120f] = 0;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x120f value is 10, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x120f] = 10;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 10, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 10);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x120f value is 20, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x120f] = 20;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x120f value is 20, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x120f] = 20;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 255, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 255);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x120f value is 30, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x120f] = 30;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 246, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 246);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x120f value is -15, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x120f] = -15;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x120f value is -10, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x120f] = -10;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -5, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-5);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x120f value is -10, and "
						"carry is not set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(false);
						mem._data[0x120f] = -10;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -6, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-6);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x120f value is -10, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x120f] = -10;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 25, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 25);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x120f value is -127, and "
						"carry is set")
					{
						cpu.accumulator(127);
						cpu.c_flag(true);
						mem._data[0x120f] = -127;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -2, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -127, "
						"address 0x120f value is 127, and "
						"carry is set")
					{
						cpu.accumulator(-127);
						cpu.c_flag(true);
						mem._data[0x120f] = 127;

						WHEN("operation is performed with page cross (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 2, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 2);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}
				}
			}
		}
	}

	GIVEN("operation is sbc [indirect, x]")
	{
		mem._data[0] = 0xe1;

		AND_GIVEN(
			"address value is 0x04, "
			"x register value is 0x04, and "
			"indirect address value is 0x0115")
		{
			mem._data[1] = 0x04;
			mem._data[0x08] = 0x15;
			mem._data[0x09] = 0x01;
			cpu.x_register(0x04);

			AND_GIVEN("using unsigned numbers")
			{
				AND_GIVEN(
					"accumulator value is 0, "
					"address 0x0115 value is 0, and "
					"carry is set")
				{
					cpu.accumulator(0);
					cpu.c_flag(true);
					mem._data[0x0115] = 0;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x0115 value is 10, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x0115] = 10;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 10, "
							"c flag is set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 10);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x0115 value is 20, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x0115] = 20;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, and "
							"z flag is set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x0115 value is 20, and "
					"carry is not set")
				{
					cpu.accumulator(20);
					cpu.c_flag(false);
					mem._data[0x0115] = 20;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 255, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 255);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 20, "
					"address 0x0115 value is 30, and "
					"carry is set")
				{
					cpu.accumulator(20);
					cpu.c_flag(true);
					mem._data[0x0115] = 30;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 246, "
							"c flag is not set, and "
							"z flag is not set")
						{
							REQUIRE(cpu.accumulator() == 246);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.z_flag());
						}
					}
				}
			}

			AND_GIVEN("using signed numbers")
			{
				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x0115 value is -15, and "
					"carry is set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(true);
					mem._data[0x0115] = -15;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 0, "
							"c flag is set, "
							"v flag is not set, "
							"z flag is set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 0);
							REQUIRE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x0115 value is -10, and "
					"carry is set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(true);
					mem._data[0x0115] = -10;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of -5, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-5);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -15, "
					"address 0x0115 value is -10, and "
					"carry is not set")
				{
					cpu.accumulator(-15);
					cpu.c_flag(false);
					mem._data[0x0115] = -10;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of -6, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-6);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 15, "
					"address 0x0115 value is -10, and "
					"carry is set")
				{
					cpu.accumulator(15);
					cpu.c_flag(true);
					mem._data[0x0115] = -10;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 25, "
							"c flag is not set, "
							"v flag is not set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 25);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE_FALSE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is 127, "
					"address 0x0115 value is -127, and "
					"carry is set")
				{
					cpu.accumulator(127);
					cpu.c_flag(true);
					mem._data[0x0115] = -127;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of -2, "
							"c flag is not set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is set")
						{
							REQUIRE(cpu.accumulator() == (uint8_t)-2);
							REQUIRE_FALSE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE(cpu.n_flag());
						}
					}
				}

				AND_GIVEN(
					"accumulator value is -127, "
					"address 0x0115 value is 127, and "
					"carry is set")
				{
					cpu.accumulator(-127);
					cpu.c_flag(true);
					mem._data[0x0115] = 127;

					WHEN("operation is performed (6 cycles)")
					{
						cpu.cycle(6);

						THEN(
							"accumulator has a value of 2, "
							"c flag is set, "
							"v flag is set, "
							"z flag is not set, and "
							"n flag is not set")
						{
							REQUIRE(cpu.accumulator() == 2);
							REQUIRE(cpu.c_flag());
							REQUIRE(cpu.v_flag());
							REQUIRE_FALSE(cpu.z_flag());
							REQUIRE_FALSE(cpu.n_flag());
						}
					}
				}
			}
		}
	}

	GIVEN("operation is sbc [indirect, y]")
	{
		mem._data[0] = 0xf1;

		AND_GIVEN("address value is 0x08 and indirect address value is 0x01fe")
		{
			mem._data[1] = 0x08;
			mem._data[0x08] = 0xfe;
			mem._data[0x09] = 0x01;

			AND_GIVEN("y register value is 0x01")
			{
				cpu.y_register(0x01);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x01ff value is 0, and "
						"carry is set")
					{
						cpu.accumulator(0);
						cpu.c_flag(true);
						mem._data[0x01ff] = 0;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x01ff value is 10, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x01ff] = 10;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 10, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 10);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x01ff value is 20, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x01ff] = 20;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x01ff value is 20, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x01ff] = 20;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 255, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 255);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x01ff value is 30, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x01ff] = 30;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 246, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 246);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x01ff value is -15, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x01ff] = -15;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x01ff value is -10, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x01ff] = -10;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -5, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-5);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x01ff value is -10, and "
						"carry is not set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(false);
						mem._data[0x01ff] = -10;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -6, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-6);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x01ff value is -10, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x01ff] = -10;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 25, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 25);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x01ff value is -127, and "
						"carry is set")
					{
						cpu.accumulator(127);
						cpu.c_flag(true);
						mem._data[0x01ff] = -127;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of -2, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -127, "
						"address 0x01ff value is 127, and "
						"carry is set")
					{
						cpu.accumulator(-127);
						cpu.c_flag(true);
						mem._data[0x01ff] = 127;

						WHEN("operation is performed (5 cycles)")
						{
							cpu.cycle(5);

							THEN(
								"accumulator has a value of 2, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 2);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}
				}
			}

			AND_GIVEN("y register value is 0x02")
			{
				cpu.y_register(0x02);

				AND_GIVEN("using unsigned numbers")
				{
					AND_GIVEN(
						"accumulator value is 0, "
						"address 0x0200 value is 0, and "
						"carry is set")
					{
						cpu.accumulator(0);
						cpu.c_flag(true);
						mem._data[0x0200] = 0;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x0200 value is 10, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x0200] = 10;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 10, "
								"c flag is set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 10);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x0200 value is 20, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x0200] = 20;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, and "
								"z flag is set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x0200 value is 20, and "
						"carry is not set")
					{
						cpu.accumulator(20);
						cpu.c_flag(false);
						mem._data[0x0200] = 20;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 255, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 255);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 20, "
						"address 0x0200 value is 30, and "
						"carry is set")
					{
						cpu.accumulator(20);
						cpu.c_flag(true);
						mem._data[0x0200] = 30;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 246, "
								"c flag is not set, and "
								"z flag is not set")
							{
								REQUIRE(cpu.accumulator() == 246);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.z_flag());
							}
						}
					}
				}

				AND_GIVEN("using signed numbers")
				{
					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x0200 value is -15, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x0200] = -15;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 0, "
								"c flag is set, "
								"v flag is not set, "
								"z flag is set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 0);
								REQUIRE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x0200 value is -10, and "
						"carry is set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(true);
						mem._data[0x0200] = -10;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of -5, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-5);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -15, "
						"address 0x0200 value is -10, and "
						"carry is not set")
					{
						cpu.accumulator(-15);
						cpu.c_flag(false);
						mem._data[0x0200] = -10;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of -6, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-6);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 15, "
						"address 0x0200 value is -10, and "
						"carry is set")
					{
						cpu.accumulator(15);
						cpu.c_flag(true);
						mem._data[0x0200] = -10;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 25, "
								"c flag is not set, "
								"v flag is not set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 25);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE_FALSE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is 127, "
						"address 0x0200 value is -127, and "
						"carry is set")
					{
						cpu.accumulator(127);
						cpu.c_flag(true);
						mem._data[0x0200] = -127;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of -2, "
								"c flag is not set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is set")
							{
								REQUIRE(cpu.accumulator() == (uint8_t)-2);
								REQUIRE_FALSE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE(cpu.n_flag());
							}
						}
					}

					AND_GIVEN(
						"accumulator value is -127, "
						"address 0x0200 value is 127, and "
						"carry is set")
					{
						cpu.accumulator(-127);
						cpu.c_flag(true);
						mem._data[0x0200] = 127;

						WHEN("operation is performed with page cross(6 cycles)")
						{
							cpu.cycle(6);

							THEN(
								"accumulator has a value of 2, "
								"c flag is set, "
								"v flag is set, "
								"z flag is not set, and "
								"n flag is not set")
							{
								REQUIRE(cpu.accumulator() == 2);
								REQUIRE(cpu.c_flag());
								REQUIRE(cpu.v_flag());
								REQUIRE_FALSE(cpu.z_flag());
								REQUIRE_FALSE(cpu.n_flag());
							}
						}
					}
				}
			}
		}
	}
}
