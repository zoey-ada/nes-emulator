#include "randomAccessMemory.hpp"

RandomAccessMemory::RandomAccessMemory(uint16_t size): _size(size)
{
	this->_data = new uint8_t[size];
}

RandomAccessMemory::~RandomAccessMemory()
{
	if (this->_data != nullptr)
		delete[] this->_data;
	this->_data = nullptr;
}

void RandomAccessMemory::dumpMemory(std::ostream& outstream)
{
	for (int i = 0; i < this->_size; ++i)
		outstream << this->_data[i];
}
