#pragma once

#include <cstdint>

#include "iInput.hpp"

class Controller: public IInput
{
public:
	Controller();
	virtual ~Controller();

	bool read() override;
	void write(const bool data);

	void readInputs();
	bool readAButtonInput();

private:
	bool _read_inputs_latch {false};
	uint8_t _data_latch {0x00};
	uint8_t _current_bit {0};
};
