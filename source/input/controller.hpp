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

	virtual void readInputs() = 0;
	virtual bool readAButtonInput() = 0;

protected:
	uint8_t _data_latch {0x00};

private:
	bool _read_inputs_latch {false};
	uint8_t _current_bit {0};
};
