#include "nes.hpp"

Nes::Nes()
{
	this->_cart_loader = std::make_unique<CartridgeLoader>();
	// std::string cart_name = "c:/Users/zoeya/Downloads/Super Mario Bros (E).nes";
	// this->_cart = this->_cart_loader->load_cartridge(cart_name);

	this->_ppu_memory = std::make_unique<PpuMemoryMapper>();
	// this->_ppu_memory->load_cartridge(this->_cart.get());
	this->_ppu = std::make_unique<PictureProcessingUnit>(this->_ppu_memory.get());

	this->_memory = std::make_unique<MemoryMapper>(this->_ppu.get());
	// this->_memory->load_cartridge(this->_cart.get());
	this->_cpu = std::make_unique<Cpu>(this->_memory.get());
	this->_ppu->init(this->_cpu.get());

	this->_ppu->power_up();
	this->_cpu->power_up();

	this->_left = std::make_unique<PatternTable>(false);
	// this->_left->loadCartridge(this->_cart.get());
	this->_right = std::make_unique<PatternTable>(true);
	// this->_right->loadCartridge(this->_cart.get());
}

void Nes::produceFrame()
{
	this->produceNesFrame();
	this->_left->produceFrame();
	this->_right->produceFrame();
}

void Nes::produceNesFrame()
{
	if (!this->_cart)
	{
		for (auto& pixel : this->_frame)
		{
			pixel.r = 0x00;
			pixel.g = 0x00;
			pixel.b = 0x00;
		}

		return;
	}

	static uint32_t even_cycles = 89342;
	static uint64_t odd_cycles = 89341;
	static bool is_even_frame = false;

	if (is_even_frame)
	{
		for (auto i = 0; i < even_cycles; ++i)
		{
			this->_ppu->cycle();

			auto pixel = ((NesPixel*)&this->_frame[i]);
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

			auto pixel = ((NesPixel*)&this->_frame[i]);
			*((uint32_t*)pixel) = this->_ppu->vout();

			// if (i % 3 == 2)
			// 	this->_cpu->cycle();
		}
		auto last_pixel = ((NesPixel*)&this->_frame[nes_total_pixels - 1]);
		*((uint32_t*)last_pixel) = 0x00000000;
	}

	is_even_frame = !is_even_frame;
}

void Nes::loadFile(const std::string& filepath)
{
	this->_cart = this->_cart_loader->load_cartridge(filepath);

	this->_ppu_memory->load_cartridge(this->_cart.get());
	this->_memory->load_cartridge(this->_cart.get());
	this->_left->loadCartridge(this->_cart.get());
	this->_right->loadCartridge(this->_cart.get());

	this->_ppu->power_up();
	this->_cpu->power_up();
}
