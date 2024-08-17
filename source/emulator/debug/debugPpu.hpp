#pragma once

#include <map>

#include <base/color.hpp>
#include <ppu/ppu.hpp>

#include "palette.hpp"
#include "patternTable.hpp"

class Cartridge;
struct SDL_Renderer;
struct SDL_Texture;

class DebugPpu: public PictureProcessingUnit
{
public:
	DebugPpu(IMemory* memory, SDL_Renderer* renderer);
	virtual ~DebugPpu();

	SDL_Texture* leftPtTexture() const { return this->_left_pattern_table.getTexture(); }
	SDL_Texture* rightPtTexture() const { return this->_right_pattern_table.getTexture(); }

	void loadCartridge(Cartridge* cart);

private:
	PaletteType _current_palette {PaletteType::Grayscale};
	PatternTable _left_pattern_table;
	PatternTable _right_pattern_table;

	std::map<PaletteType, Palette> _palettes;

	SDL_Renderer* _renderer;

	void drawPatternTables();
	void loadPalettes();
};
