#include "ppu.hpp"

#include <cassert>

#include <memory/iMemory.hpp>
#include <memory/randomAccessMemory.hpp>

PictureProcessingUnit::PictureProcessingUnit(IMemory* memory)
{
	assert(memory != nullptr);
	this->_memory = memory;

	this->_object_attribute_memory = new (std::nothrow) RandomAccessMemory(0x0100);
}

PictureProcessingUnit::~PictureProcessingUnit()
{
	this->_memory = nullptr;
}

void PictureProcessingUnit::cycle()
{
	// auto action = this->_actions.front();
	// this->_actions.pop_front();
	// action();

	// 	prerender scanline (-1)
	// 	visible scanlines (0-239)
	//		nothing [0]
	//		fetch tile data: [1-256] {8 each}
	//			- nametable byte {2}
	//			- attribute byte {2}
	//			- pattern table tile low {2}
	//			- pattern table tile high {2}
	//		fetch tile data for sprites on next line: [257-320] {8 each}
	//			- garbage nametable byte {2}
	//			- garbage nametable byte {2} && grab attribute and x pos data for sprite from oam
	//			- pattern table tile low {2}
	//			- pattern table tile high {2}
	//		fetch tile data for tile 1+2 of next line: [321-336] {8 each}
	//			- nametable byte {2}
	//			- attribute byte {2}
	//			- pattern table tile low {2}
	//			- pattern table tile high {2}
	//		fetch unused bytes [337-340]
	//			- nametable byte {2}
	//			- nametable byte {2}
	//	post render scanline (240)
	//		- idle
	//	vertical blanking lines (241-260)
	//		- vblank flag is set on second tick of line 241 (and vblank nmi?)
	//		- no mem accessed, so app can update ppu mem

	// red = 0x00FF0000;
	// green = 0x0000FF00;
	// blue = 0x000000FF;
	// output in rgb (0x000000)

	if (_row > 240)
		this->_vout = 0x00FF00;
	else if (_column < 256)
		this->_vout = 0x0000FF;
	else
		this->_vout = 0xFF0000;

	if ((!this->_is_even_frame) && this->_row == 261 && this->_column == 339)
	{
		this->_column = 0;
		this->_row = 0;
		this->_is_even_frame = true;
		return;
		// this->_column++;
	}

	this->_column = (this->_column + 1) % 341;
	if (this->_column == 0)
		this->_row = (this->_row + 1) % 262;
	if (this->_row == 0)
	{
		this->_is_even_frame = !this->_is_even_frame;
	}
}

void PictureProcessingUnit::clear_registers()
{}

void PictureProcessingUnit::power_up()
{}

void PictureProcessingUnit::cycle(uint8_t number_cycles)
{
	for (auto i = 0; i < number_cycles; ++i)
		this->cycle();
}

// 262 scanlines per frame
// 341 clock cycles/pixels per scanline

uint16_t PictureProcessingUnit::calculate_pattern_table_address(uint8_t row_of_tile,
	uint8_t nametable_entry)
{
	// DCBA98 76543210
	// ---------------
	// 0HRRRR CCCCPTTT
	// |||||| |||||+++- T: Fine Y offset, the row number within a tile
	// |||||| ||||+---- P: Bit plane (0: "lower"; 1: "upper")
	// |||||| ++++----- C: Tile column
	// ||++++---------- R: Tile row
	// |+-------------- H: Half of pattern table (0: "left"; 1: "right")
	// +--------------- 0: Pattern table is at $0000-$1FFF

	auto pattern_table_address = 0x0000;

	bool use_right_pattern_table = (this->_ppuctrl & 0b00100000) > 0;
	if (use_right_pattern_table)
		pattern_table_address |= 0b0001000000000000;

	pattern_table_address += (nametable_entry << 4);
	pattern_table_address += row_of_tile;

	return pattern_table_address;
}

void PictureProcessingUnit::fetch_background_tile_data(uint8_t current_row)
{
	const uint8_t tiles_in_row = 32;
	// Fetch a nametable entry from $2000-$2FBF.
	// Fetch the corresponding attribute table entry from $23C0-$2FFF and
	//		increment the current VRAM address within the same row.
	// Fetch the low-order byte of an 8x1 pixel sliver of pattern table from
	//		$0000-$0FF7 or $1000-$1FF7.
	// Fetch the high-order byte of this sliver from an address 8 bytes higher.
	// Turn the attribute data and the pattern table data into palette indices,
	//		and combine them with data from sprite data using priority.

	bool use_right_pattern_table = (this->_ppuctrl & 0b00100000) > 0;
	uint8_t tile_row = current_row % 8;
	uint8_t tile_y_index = current_row / 8;

	uint16_t nametable_offset = 0x2000;
	uint8_t nametable_number = 0;
	nametable_offset += 0x0400;
	nametable_offset += tile_row * tiles_in_row;
	uint16_t attribute_table_offset = nametable_offset + 0x03c0;

	// for (uint8_t tile_x_index = 0; tile_index < tiles_in_row; ++tile_index)
	// {
	// 	this->_actions.push_back([this]() {
	// 		auto nametable_entry = this->_memory->read(nametable_offset + tile_index);
	// 		auto attributes = this->_memory->read(attribute_table_offset + tile_index / 4);

	// 		auto pattern_table_address =
	// 			this->calculate_pattern_table_address(tile_row, nametable_entry);
	// 		auto low_byte_pixels = this->_memory->read(pattern_table_address);
	// 		auto high_byte_pixels = this->_memory->read(pattern_table_address + 8);
	// 	});
	// }
}

void PictureProcessingUnit::fetch_sprite_tile_data()
{}
