#include "nes.hpp"

Nes::Nes(SDL_Renderer* renderer)
{
	this->_cart_loader = std::make_unique<CartridgeLoader>();
	// std::string cart_name = "c:/Users/zoeya/Downloads/Super Mario Bros (E).nes";
	// this->_cart = this->_cart_loader->load_cartridge(cart_name);

	this->_ppu_memory = std::make_unique<PpuMemoryMapper>();
	this->_ppu = std::make_unique<PictureProcessingUnit>(this->_ppu_memory.get());

	this->_memory = std::make_unique<MemoryMapper>(this->_ppu.get());
	// this->_cpu = std::make_unique<Cpu>(this->_memory.get());
	// this->_ppu->init(this->_cpu.get());
	this->_debug_cpu = std::make_unique<DebugCpu>(this->_memory.get());
	this->_ppu->init(this->_debug_cpu.get());

	// this->_ppu->power_up();
	// this->_cpu->power_up();
	// this->_debug_cpu->power_up();

	this->_left = std::make_unique<PatternTable>(false);
	this->_right = std::make_unique<PatternTable>(true);
	this->_cpu_renderer = std::make_unique<CpuRenderer>(renderer);

	this->blankFrame();
}

void Nes::produceFrame()
{
	this->renderDebugImages();
	// this->produceNesFrame();
	// this->_left->produceFrame();
	// this->_right->produceFrame();
	// auto cycle_data = this->_debug_cpu->getLastStackFrame();
	// this->_cpu_renderer->produceFrame(cycle_data);
}

void Nes::produceNesFrame()
{
	// if (!this->_cart)
	// {
	// 	for (auto& pixel : this->_frame)
	// 	{
	// 		pixel.r = 0x00;
	// 		pixel.g = 0x00;
	// 		pixel.b = 0x00;
	// 	}

	// 	return;
	// }

	// static uint32_t even_cycles = 89342;
	// static uint64_t odd_cycles = 89341;
	// static bool is_even_frame = false;

	// if (is_even_frame)
	// {
	// 	for (auto i = 0; i < even_cycles; ++i)
	// 	{
	// 		this->_ppu->cycle();

	// 		auto pixel = ((Pixel*)&this->_frame[i]);
	// 		*((uint32_t*)pixel) = this->_ppu->vout();

	// 		// if (i % 3 == 2)
	// 		// 	// this->_cpu->cycle();
	// 		// 	this->_debug_cpu->cycle();
	// 	}
	// }
	// else
	// {
	// 	for (auto i = 0; i < odd_cycles; ++i)
	// 	{
	// 		this->_ppu->cycle();

	// 		auto pixel = ((Pixel*)&this->_frame[i]);
	// 		*((uint32_t*)pixel) = this->_ppu->vout();

	// 		// if (i % 3 == 2)
	// 		// 	// this->_cpu->cycle();
	// 		// 	this->_debug_cpu->cycle();
	// 	}
	// 	auto last_pixel = ((Pixel*)&this->_frame[nes_total_pixels - 1]);
	// 	*((uint32_t*)last_pixel) = 0x00000000;
	// }

	// is_even_frame = !is_even_frame;
}

void Nes::loadFile(const std::string& filepath)
{
	this->_cart = this->_cart_loader->load_cartridge(filepath);

	this->_ppu_memory->load_cartridge(this->_cart.get());
	this->_memory->load_cartridge(this->_cart.get());
	this->_left->loadCartridge(this->_cart.get());
	this->_right->loadCartridge(this->_cart.get());

	this->_ppu->power_up();
	// this->_cpu->power_up();
	this->_debug_cpu->power_up();

	this->_game_loaded = true;

	this->_cpu_cycle_delay = cpu_cycle_delay;
	this->_current_cycle = 0;
	this->_is_even_frame = false;
}

void Nes::step()
{
	if (!this->_cart)
		return;

	this->cycle(3);
}

void Nes::leap()
{
	if (!this->_cart)
		return;

	this->cycle(3 * 10);
}

void Nes::bound()
{
	if (!this->_cart)
		return;

	this->cycle(3 * 10 * 100);
}

void Nes::cycle()
{
	if (!this->_is_even_frame && this->_current_cycle == (odd_cycles))
	{
		auto last_pixel = ((Pixel*)&this->_frame[nes_total_pixels - 1]);
		*((uint32_t*)last_pixel) = 0x00000000;

		this->resetCurrentCycle();
	}
	else if (this->_is_even_frame && this->_current_cycle == (even_cycles))
	{
		this->resetCurrentCycle();
	}

	this->_ppu->cycle();

	auto pixel = ((Pixel*)&this->_frame[this->_current_cycle]);
	*((uint32_t*)pixel) = this->_ppu->vout();

	this->_current_cycle++;
	this->_cpu_cycle_delay--;

	if (this->_cpu_cycle_delay == 0)
	{
		this->_debug_cpu->cycle();
		this->_cpu_cycle_delay = cpu_cycle_delay;
	}
}

void Nes::resetCurrentCycle()
{
	this->_is_even_frame = !this->_is_even_frame;
	this->_current_cycle = 0;
}

void Nes::renderDebugImages()
{
	this->_left->produceFrame();
	this->_right->produceFrame();
	auto cycle_data = this->_debug_cpu->getLastStackFrame();
	this->_cpu_renderer->produceFrame(cycle_data);
}

void Nes::blankFrame()
{
	for (auto& pixel : this->_frame)
	{
		pixel.r = 0x00;
		pixel.g = 0x00;
		pixel.b = 0x00;
	}
}
