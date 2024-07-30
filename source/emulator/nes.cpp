#include "nes.hpp"

Nes::Nes()
{
	this->_memory = std::make_unique<MemoryMapper>();
	this->_cpu = std::make_unique<Cpu>(this->_memory.get());
	this->_ppu = std::make_unique<PictureProcessingUnit>();

	// for (int i = 0; i < total_pixels; ++i)
	// {
	// 	auto pixel = ((Pixel*)&this->_frame[i]);

	// 	// red
	// 	pixel->r = 255;
	// 	// green
	// 	pixel->g = 2;
	// 	// blue
	// 	pixel->b = 3;
	// }
}

void Nes::produceFrame()
{
	static uint32_t even_cycles = 89342;
	static uint64_t odd_cycles = 89341;
	static bool is_even_frame = false;

	if (is_even_frame)
	{
		for (auto i = 0; i < even_cycles; ++i)
		{
			this->_ppu->cycle();

			auto pixel = ((Pixel*)&this->_frame[i]);
			*((uint32_t*)pixel) = this->_ppu->vout();

			// if (i % 3 == 2)
			// 	this->_cpu->cycle();
		}
	}
	else
	{
		for (auto i = 0; i < odd_cycles; ++i)
		{
			this->_ppu->cycle();

			auto pixel = ((Pixel*)&this->_frame[i]);
			*((uint32_t*)pixel) = this->_ppu->vout();

			// if (i % 3 == 2)
			// 	this->_cpu->cycle();
		}
		auto last_pixel = ((Pixel*)&this->_frame[total_pixels - 1]);
		*((uint32_t*)last_pixel) = 0x00000000;
	}

	is_even_frame = !is_even_frame;

	// for (uint16_t cycle = 0; cycle < 89342)
	// 	this->_cpu->cycle(89342 / 3);
	// this->_ppu->cycle(89342);
}
