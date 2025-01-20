#include "debugPpu.hpp"

#include <base/iMemory.hpp>
#include <cartridge/cartridge.hpp>
#include <ppu/ppuMemoryMapper.hpp>

DebugPpu::DebugPpu(IMemory* memory, IMemory* oam, std::shared_ptr<IRenderer> renderer)
	: PictureProcessingUnit(memory, oam),
	  _renderer(renderer),
	  _palette_renderer(renderer),
	  _left_pattern_table(PatternTableType::Left, renderer),
	  _right_pattern_table(PatternTableType::Right, renderer)
{
	this->_palettes[PaletteType::Grayscale] = greyscale_palette;
}

DebugPpu::~DebugPpu()
{
	this->_renderer = nullptr;
}

void DebugPpu::loadCartridge(Cartridge* cart)
{
	this->_cart = cart;
	this->_left_pattern_table.loadCartridge(cart);
	this->_right_pattern_table.loadCartridge(cart);

	this->loadPalettes();
	this->_palette_renderer.renderPalettes(this->_palettes, this->_current_palette);
}

void DebugPpu::nextPalette()
{
	auto palette_num = static_cast<int>(this->_current_palette);
	palette_num = (palette_num + 1) % 9;
	this->_current_palette = PaletteType(palette_num);

	this->_left_pattern_table.loadPalette(this->_palettes[this->_current_palette]);
	this->_right_pattern_table.loadPalette(this->_palettes[this->_current_palette]);
	this->_palette_renderer.renderPalettes(this->_palettes, this->_current_palette);
}

void DebugPpu::prevPalette()
{
	auto palette_num = static_cast<int>(this->_current_palette);
	palette_num = (palette_num - 1 + NumberPalettes) % 9;
	this->_current_palette = PaletteType(palette_num);

	this->_left_pattern_table.loadPalette(this->_palettes[this->_current_palette]);
	this->_right_pattern_table.loadPalette(this->_palettes[this->_current_palette]);
	this->_palette_renderer.renderPalettes(this->_palettes, this->_current_palette);
}

void DebugPpu::dumpMemory()
{
	static_cast<PpuMemoryMapper*>(this->_memory)->dumpPaletteRam();
	static_cast<PpuMemoryMapper*>(this->_memory)->dumpVideoRam();
}

void DebugPpu::drawPatternTables()
{
	this->_left_pattern_table.draw();
	this->_right_pattern_table.draw();
}

void DebugPpu::loadPalettes()
{
	Color shared_color = this->_sys_palette->getColor(this->_memory->read(0x3f00));

	this->_palettes[PaletteType::Background0] = {
		shared_color,
		this->_sys_palette->getColor(this->_memory->read(0x3f01)),
		this->_sys_palette->getColor(this->_memory->read(0x3f02)),
		this->_sys_palette->getColor(this->_memory->read(0x3f03)),
	};

	this->_palettes[PaletteType::Background1] = {
		shared_color,
		this->_sys_palette->getColor(this->_memory->read(0x3f05)),
		this->_sys_palette->getColor(this->_memory->read(0x3f06)),
		this->_sys_palette->getColor(this->_memory->read(0x3f07)),
	};

	this->_palettes[PaletteType::Background2] = {
		shared_color,
		this->_sys_palette->getColor(this->_memory->read(0x3f09)),
		this->_sys_palette->getColor(this->_memory->read(0x3f0a)),
		this->_sys_palette->getColor(this->_memory->read(0x3f0b)),
	};

	this->_palettes[PaletteType::Background3] = {
		shared_color,
		this->_sys_palette->getColor(this->_memory->read(0x3f0d)),
		this->_sys_palette->getColor(this->_memory->read(0x3f0e)),
		this->_sys_palette->getColor(this->_memory->read(0x3f0f)),
	};

	this->_palettes[PaletteType::Sprite0] = {
		shared_color,
		this->_sys_palette->getColor(this->_memory->read(0x3f11)),
		this->_sys_palette->getColor(this->_memory->read(0x3f02)),
		this->_sys_palette->getColor(this->_memory->read(0x3f03)),
	};

	this->_palettes[PaletteType::Sprite1] = {
		shared_color,
		this->_sys_palette->getColor(this->_memory->read(0x3f15)),
		this->_sys_palette->getColor(this->_memory->read(0x3f06)),
		this->_sys_palette->getColor(this->_memory->read(0x3f07)),
	};

	this->_palettes[PaletteType::Sprite2] = {
		shared_color,
		this->_sys_palette->getColor(this->_memory->read(0x3f19)),
		this->_sys_palette->getColor(this->_memory->read(0x3f0a)),
		this->_sys_palette->getColor(this->_memory->read(0x3f0b)),
	};

	this->_palettes[PaletteType::Sprite3] = {
		shared_color,
		this->_sys_palette->getColor(this->_memory->read(0x3f1d)),
		this->_sys_palette->getColor(this->_memory->read(0x3f0e)),
		this->_sys_palette->getColor(this->_memory->read(0x3f0f)),
	};
}

void DebugPpu::write_memory()
{
	PictureProcessingUnit::write_memory();

	if (this->_cart->usesCharacterRam() && this->_address_bus() < 0x2000)
	{
		this->drawPatternTables();
	}
	else if (this->_address_bus() >= 0x3f00)
	{
		this->loadPalettes();
		this->_left_pattern_table.loadPalette(this->_palettes[this->_current_palette]);
		this->_right_pattern_table.loadPalette(this->_palettes[this->_current_palette]);
		this->_palette_renderer.renderPalettes(this->_palettes, this->_current_palette);
	}
	// else if (this->_address_bus() >= 0x23c0 and this->_address_bus() < 0x2400)
	// {
	// 	auto address = this->_address_bus();
	// 	auto value = this->_data_bus();
	// 	auto var = false;
	// }
}
