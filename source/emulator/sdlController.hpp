#pragma once

#include <input/controller.hpp>

class SdlController: public Controller
{
public:
	SdlController() = default;
	virtual ~SdlController() = default;

	void readInputs() override;
	bool readAButtonInput() override;
};
