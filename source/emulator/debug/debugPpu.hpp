#pragma once

#include <map>
#include <memory>

#include <base/color.hpp>
#include <platform/iRenderer.hpp>
#include <ppu/ppu.hpp>

#include "palette.hpp"
#include "patternTable.hpp"

class Cartridge;

const int NumberPalettes = 9;

class DebugPpu: public PictureProcessingUnit
{
public:
	DebugPpu(IMemory* memory, IMemory* oam, std::shared_ptr<IRenderer> renderer);
	virtual ~DebugPpu();

	Texture leftPtTexture() const { return this->_left_pattern_table.getTexture(); }
	Texture rightPtTexture() const { return this->_right_pattern_table.getTexture(); }

	void loadCartridge(Cartridge* cart);

	void nextPalette();
	void prevPalette();

	void dumpMemory();

private:
	PaletteType _current_palette {PaletteType::Grayscale};
	PatternTable _left_pattern_table;
	PatternTable _right_pattern_table;

	std::map<PaletteType, Palette> _palettes;

	std::shared_ptr<IRenderer> _renderer;
	Cartridge* _cart;

	void drawPatternTables();
	void loadPalettes();

	void write_memory() override;
};
