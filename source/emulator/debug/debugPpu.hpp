#pragma once

#include <map>
#include <memory>

#include <base/color.hpp>
#include <platform/iRenderer.hpp>
#include <ppu/ppu.hpp>

#include "palette.hpp"
#include "paletteRenderer.hpp"
#include "patternTable.hpp"
#include "spriteTable.hpp"

class Cartridge;

const int NumberPalettes = 9;

class DebugPpu: public PictureProcessingUnit
{
public:
	DebugPpu(IMemory* memory, IMemory* oam, std::shared_ptr<IRenderer> renderer);
	virtual ~DebugPpu();

	Texture spriteTableTexture() const { return this->_sprite_table.getTexture(); }
	Texture leftPtTexture() const { return this->_left_pattern_table.getTexture(); }
	Texture rightPtTexture() const { return this->_right_pattern_table.getTexture(); }
	Texture paletteTableTexture() const { return this->_palette_renderer.getTexture(); }

	void loadCartridge(Cartridge* cart);

	void nextPalette();
	void prevPalette();

	void dumpMemory();

	void oam_data(const uint8_t value);

	void drawSpriteTable();

private:
	PaletteType _current_palette {PaletteType::Grayscale};
	SpriteTable _sprite_table;
	PatternTable _left_pattern_table;
	PatternTable _right_pattern_table;
	PaletteRenderer _palette_renderer;

	std::map<PaletteType, Palette> _palettes;

	std::shared_ptr<IRenderer> _renderer;
	Cartridge* _cart;

	void drawPatternTables();
	void loadPalettes();

	void write_memory() override;
};
