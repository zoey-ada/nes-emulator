#include "nes.hpp"

#include <base/slowMemory.hpp>

Nes::Nes(std::shared_ptr<IRenderer> renderer)
{
	if (this->_debug_mode)
		this->setupDebugNes(renderer);
	else
		this->setupNes();

	// std::string cart_name = "c:/Users/zoeya/Downloads/Super Mario Bros (E).nes";
	// this->_cart = this->_cart_loader->load_cartridge(cart_name);

	this->blankFrame();
}

void Nes::produceNesFrame()
{}

Texture Nes::getLeftPtTexture() const
{
	if (this->_debug_mode)
		return this->_debug_ppu->leftPtTexture();
	else
		return nullptr;
}

Texture Nes::getRightPtTexture() const
{
	if (this->_debug_mode)
		return this->_debug_ppu->rightPtTexture();
	else
		return nullptr;
}

Texture Nes::getCpuDebugTexture() const
{
	if (this->_debug_mode)
		return this->_debug_cpu->getTexture();
	else
		return nullptr;
}

void Nes::loadFile(const std::string& filepath)
{
	this->_cart = this->_cart_loader->load_cartridge(filepath);

	this->_ppu_memory->load_cartridge(this->_cart.get());
	this->_cpu_memory->load_cartridge(this->_cart.get());

	if (this->_debug_mode)
	{
		this->_debug_ppu->loadCartridge(this->_cart.get());
		this->_debug_ppu->reset();
		this->_debug_cpu->reset();
	}
	else
	{
		this->_ppu->reset();
		this->_cpu->reset();
	}

	this->_game_loaded = true;

	this->_cpu_cycle_delay = cpu_cycle_delay;
	this->_current_cycle = 0;
	this->_is_even_frame = false;
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

	if (this->_debug_mode)
	{
		this->_debug_ppu->cycle();
		auto pixel = ((Pixel*)&this->_frame[this->_current_cycle]);
		*((uint32_t*)pixel) = this->_debug_ppu->vout();
	}
	else
	{
		this->_ppu->cycle();
		auto pixel = ((Pixel*)&this->_frame[this->_current_cycle]);
		*((uint32_t*)pixel) = this->_ppu->vout();
	}

	this->_current_cycle++;
	this->_cpu_cycle_delay--;

	if (this->_cpu_cycle_delay == 0)
	{
		if (this->_debug_mode)
			this->_debug_cpu->cycle();
		else
			this->_cpu->cycle();

		this->_dma->cycle();

		this->_cpu_cycle_delay = cpu_cycle_delay;
	}
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

void Nes::nextFrame()
{
	auto frame_cycles = this->_is_even_frame ? even_cycles : odd_cycles;
	auto remaining_cycles = frame_cycles - this->_current_cycle;

	if (remaining_cycles == 0)
	{
		this->cycle();
		frame_cycles = this->_is_even_frame ? even_cycles : odd_cycles;
		remaining_cycles = frame_cycles - this->_current_cycle;
	}

	this->cycle(remaining_cycles);
}

void Nes::nextPalette()
{
	if (this->_debug_mode)
		this->_debug_ppu->nextPalette();
}

void Nes::prevPalette()
{
	if (this->_debug_mode)
		this->_debug_ppu->prevPalette();
}

void Nes::dumpMemory()
{
	if (this->_debug_mode)
		this->_debug_ppu->dumpMemory();
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

void Nes::resetCurrentCycle()
{
	this->_is_even_frame = !this->_is_even_frame;
	this->_current_cycle = 0;
}

void Nes::setupDebugNes(std::shared_ptr<IRenderer> renderer)
{
	this->_cart_loader = std::make_unique<CartridgeLoader>();

	this->_oam = std::make_unique<SlowMemory>(0x0100);
	this->_dma = std::make_unique<DirectMemoryAccess>();
	this->_ppu_memory = std::make_unique<PpuMemoryMapper>();
	this->_debug_ppu =
		std::make_unique<DebugPpu>(this->_ppu_memory.get(), this->_oam.get(), renderer);
	this->_cpu_memory = std::make_unique<MemoryMapper>(this->_debug_ppu.get(), this->_dma.get());
	this->_debug_cpu = std::make_unique<DebugCpu>(this->_cpu_memory.get(), renderer);
	this->_debug_ppu->init(this->_debug_cpu.get());
	this->_dma->initialize(this->_debug_cpu.get(), this->_cpu_memory.get(), this->_oam.get());

	this->_p1_controller = std::make_unique<SdlController>();
	this->_cpu_memory->connect_controller(ControllerPort::Port1, this->_p1_controller.get());
}

void Nes::setupNes()
{
	this->_cart_loader = std::make_unique<CartridgeLoader>();

	this->_oam = std::make_unique<SlowMemory>(0x0100);
	this->_dma = std::make_unique<DirectMemoryAccess>();
	this->_ppu_memory = std::make_unique<PpuMemoryMapper>();
	this->_ppu = std::make_unique<PictureProcessingUnit>(this->_ppu_memory.get(), this->_oam.get());
	this->_cpu_memory = std::make_unique<MemoryMapper>(this->_ppu.get(), this->_dma.get());
	this->_cpu = std::make_unique<Cpu>(this->_cpu_memory.get());
	this->_ppu->init(this->_cpu.get());
	this->_dma->initialize(this->_cpu.get(), this->_cpu_memory.get(), this->_oam.get());

	this->_p1_controller = std::make_unique<SdlController>();
	this->_cpu_memory->connect_controller(ControllerPort::Port1, this->_p1_controller.get());
}
