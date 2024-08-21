#pragma once

class IClocked
{
public:
	virtual ~IClocked() = default;

	virtual void cycle() = 0;
};
