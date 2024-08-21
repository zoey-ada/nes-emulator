#include "controller.hpp"

Controller::Controller()
{}

Controller::~Controller()
{}

bool Controller::read()
{
	if (this->_read_inputs_latch)
		return this->readAButtonInput();

	if (this->_current_bit > 7)
		return true;

	bool data = (this->_data_latch & (1 << _current_bit)) > 0;
	++this->_current_bit;
	return data;
}

void Controller::write(const bool data)
{
	if (!data && this->_read_inputs_latch)
		this->readInputs();

	this->_read_inputs_latch = data;
}

void Controller::readInputs()
{
	// A, B, Select, Start, Up, Down, Left, Right.
	this->_data_latch = 0b1111'1111;
}

bool Controller::readAButtonInput()
{
	return true;
}
