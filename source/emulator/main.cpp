#include <SDL.h>
#include <cpu/cpu.hpp>
#include <memory/randomAccessMemory.hpp>

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
