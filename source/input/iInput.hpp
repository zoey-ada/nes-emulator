#pragma once

#include <cstdint>

class IInput
{
public:
	virtual ~IInput() = default;

	virtual bool read() = 0;
	virtual void write(const bool data) = 0;
};
