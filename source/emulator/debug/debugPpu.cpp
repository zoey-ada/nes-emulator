#include "debugPpu.hpp"

#include <SDL.h>

DebugPpu::DebugPpu(IMemory* memory, SDL_Renderer* renderer)
	: PictureProcessingUnit(memory),
	  _renderer(renderer),
	  _left_pattern_table(PatternTableType::Left, renderer),
	  _right_pattern_table(PatternTableType::Right, renderer)
{}

DebugPpu::~DebugPpu()
{
	this->_renderer = nullptr;
}

void DebugPpu::loadCartridge(Cartridge* cart)
{
	this->_left_pattern_table.loadCartridge(cart);
	this->_right_pattern_table.loadCartridge(cart);
}

void DebugPpu::drawPatternTables()
{
	this->_left_pattern_table.draw();
	this->_right_pattern_table.draw();
}

void DebugPpu::loadPalettes()
{}
