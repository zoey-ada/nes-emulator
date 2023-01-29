#include <SDL.h>
#include <cpu/cpu.hpp>
#include <cpu/randomAccessMemory.hpp>

#include "window.hpp"

int main(int argv, char** args)
{
	// auto ram = new RandomAccessMemory(0x401f);
	// Cpu cpu(ram);
	Window window;
	window.open();
	window.run();
	window.close();

	return 0;
}
