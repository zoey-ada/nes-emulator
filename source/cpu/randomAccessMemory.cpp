#include "randomAccessMemory.hpp"

RandomAccessMemory::RandomAccessMemory(uint16_t size)
{
	this->_data = new uint8_t[size];
}

RandomAccessMemory::~RandomAccessMemory()
{
	if (this->_data != nullptr)
		delete[] this->_data;
	this->_data = nullptr;
}
