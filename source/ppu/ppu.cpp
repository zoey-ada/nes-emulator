#include "ppu.hpp"

#include <cassert>

#include <memory/iMemory.hpp>
#include <memory/randomAccessMemory.hpp>

PictureProcessingUnit::PictureProcessingUnit(IMemory* memory): _memory(memory)
{
	assert(this->_memory);
	this->_object_attribute_memory = new (std::nothrow) RandomAccessMemory(0x0100);
	this->_palette = std::make_unique<SystemPalette>();
}

PictureProcessingUnit::~PictureProcessingUnit()
{
	this->_cpu = nullptr;
	this->_memory = nullptr;

	if (this->_object_attribute_memory)
		delete this->_object_attribute_memory;
	this->_object_attribute_memory = nullptr;
}

void PictureProcessingUnit::init(Cpu* cpu)
{
	this->_cpu = cpu;
	assert(this->_cpu);
}

void PictureProcessingUnit::cycle()
{
	// if (this->_is_starting_up)
	// {
	// 	this->_startup_delay--;
	// 	if (this->_startup_delay == 0)
	// 		this->_is_starting_up = false;
	// 	return;
	// }

	if (this->_actions.empty())
		this->load_next_operation();

	auto action = this->_actions.front();
	this->_actions.pop_front();
	action();
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
{
	this->_actions.clear();
	this->_row = 0;
	this->_column = 0;
	this->_is_even_frame = false;
	this->_vram_address(0x0000);
	this->_temp_vram_address(0x0000);
}

void PictureProcessingUnit::cycle(uint8_t number_cycles)
{
	for (auto i = 0; i < number_cycles; ++i)
		this->cycle();
}

void PictureProcessingUnit::ppu_ctrl(const uint8_t value)
{
	this->_ppuctrl(value);

	uint8_t nt_bits = (value & 0b0000'0011) << 2;
	uint8_t nt_cleared = this->_temp_vram_address.highByte() & 0b0111'0011;
	uint8_t nt_set = nt_cleared | nt_bits;
	this->_temp_vram_address.highByte(nt_set);
}

uint8_t PictureProcessingUnit::ppu_status()
{
	this->_second_write_flag = false;
	return this->_ppustatus();
}

void PictureProcessingUnit::ppu_scroll(const uint8_t value)
{
	if (!this->_second_write_flag)
	{
		this->_fine_x_scroll(value);

		uint8_t addr_bits = (value & 0b1111'1000) >> 3;
		uint8_t addr_cleared = this->_temp_vram_address.lowByte() & 0b1110'0000;
		uint8_t addr_set = addr_cleared | addr_bits;
		this->_temp_vram_address.lowByte(addr_set);
	}
	else
	{
		// using the setter will only set the lowest 3 bits of the value
		this->set_fine_y_scroll(value);

		uint16_t addr_bits = static_cast<uint16_t>(value & 0b1111'1000) << 2;
		uint16_t addr_cleared = this->_temp_vram_address() & 0b0111'1100'0001'1111;
		uint16_t addr_set = addr_cleared | addr_bits;
		this->_temp_vram_address(addr_set);
	}

	this->_second_write_flag = !this->_second_write_flag;
}

void PictureProcessingUnit::ppu_addr(const uint8_t value)
{
	if (!this->_second_write_flag)
	{
		uint8_t addr_bits = value & 0b0011'1111;
		this->_temp_vram_address.highByte(addr_bits);
	}
	else
	{
		this->_temp_vram_address.lowByte(value);
		this->_vram_address(this->_temp_vram_address());
	}

	this->_second_write_flag = !this->_second_write_flag;
}

uint8_t PictureProcessingUnit::ppu_data()
{
	this->_address_bus(this->_vram_address());
	this->read_memory();
	this->increment_vram_address();
	return this->_data_bus();
}

void PictureProcessingUnit::ppu_data(const uint8_t value)
{
	this->_address_bus(this->_vram_address());
	this->_data_bus(value);
	this->write_memory();
	this->increment_vram_address();
}

void PictureProcessingUnit::load_next_operation()
{
	if (this->_row < 240)
	{
		if (!this->show_background_flag() && !this->show_sprites_flag())
		{
			this->_actions.push_back([this] { this->_vout = 0xFF'FF'FF; });
			return;
		}

		if (this->_column == 0)
		{
			this->_actions.push_back([this] { this->_vout = 0xFF'00'FF; });
		}
		else if (this->_column < 257)
		{
			this->render_chunk();
		}
		else if (this->_column < 321)
		{
			if (this->_column == 257)
				this->_actions.push_back([this] {
					this->increment_coarse_y_scroll();
					this->_vout = 0xFF'00'00;
				});
			else
				this->_actions.push_back([this] { this->_vout = 0xFF'00'00; });
			// this->fetch_sprite_data();
		}
		else if (this->_column < 340)
		{
			this->_actions.push_back([this] { this->_vout = 0xFF'00'00; });
			this->_actions.push_back([this] { this->_vout = 0xFF'00'00; });
		}
		// else
		// {
		// 	this->_column = 0;
		// 	++this->_row;
		// }
	}
	else if (this->_row == 241)
	{
		if (this->_column == 1)
		{
			this->_actions.push_back([this] {
				this->vertical_blank_flag(true);

				if (this->nmi_enable_flag())
					this->_cpu->nmi();

				this->_vout = 0x00'FF'00;
			});
		}
		else
		{
			this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
		}
	}
	else if (this->_row < 261)
	{
		this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
	}
	else
	{
		if (this->_column == 1)
		{
			this->_actions.push_back([this] {
				this->vertical_blank_flag(false);
				this->_vout = 0x00'FF'00;
			});
		}
		else if (this->_column >= 280 && this->_column <= 304)
		{
			if (!this->show_background_flag() && !this->show_sprites_flag())
			{
				this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
				return;
			}

			this->_actions.push_back([this] {
				this->reset_coarse_y_scroll();
				this->_vout = 0x00'FF'00;
			});
		}
		else
		{
			this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
		}
	}
}

// 262 scanlines per frame
// 341 clock cycles/pixels per scanline

void PictureProcessingUnit::read_memory()
{
	this->_data_bus(this->_memory->read(this->_address_bus()));
}

void PictureProcessingUnit::write_memory()
{
	this->_memory->write(this->_address_bus(), this->_data_bus());
}

void PictureProcessingUnit::feed_vout()
{
	this->_vout = this->_vout_shift_register[this->_vout_shift_register_index];
	this->_vout_shift_register_index = (this->_vout_shift_register_index + 1) % 8;
}

void PictureProcessingUnit::nametable_read()
{
	this->_actions.push_back([this] {
		this->_address_bus(this->get_nametable_address());
		this->feed_vout();
	});
	this->_actions.push_back([this] {
		this->read_memory();
		this->_nametable_latch = this->_data_bus();
		this->feed_vout();
	});
}

void PictureProcessingUnit::attribute_table_read()
{
	this->_actions.push_back([this] {
		this->_address_bus(this->get_attribute_table_address());
		this->feed_vout();
	});
	this->_actions.push_back([this] {
		this->read_memory();
		this->_attribute_table_latch = this->_data_bus();
		this->feed_vout();
	});
}

void PictureProcessingUnit::process_visible_pixels()
{
	this->_actions.push_back([this] {
		this->_address_bus(this->calculate_pattern_table_address(false));
		this->feed_vout();
	});
	this->_actions.push_back([this] {
		this->read_memory();
		this->_pattern_table_tile_low_latch = this->_data_bus();
		this->feed_vout();
	});

	this->_actions.push_back([this] {
		this->_address_bus(this->calculate_pattern_table_address(true));
		this->feed_vout();
	});
	this->_actions.push_back([this] {
		this->read_memory();
		this->_pattern_table_tile_high_latch = this->_data_bus();
		this->feed_vout();

		if (this->_column == 256)
			this->reset_coarse_x_scroll();
		else
			this->increment_coarse_x_scroll();

		auto indices = this->compile_pattern_table_bytes();
		auto pixels = this->apply_palette_colors(indices);
		this->_vout_shift_register = std::move(pixels);
		// this->determine_pixels();
		// this->fill_shift_register();
	});
}

void PictureProcessingUnit::render_chunk()
{
	if (!this->show_background_flag())
	{
		this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
		this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
		this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
		this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
		this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
		this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
		this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
		this->_actions.push_back([this] { this->_vout = 0x00'FF'00; });
		return;
	}

	this->nametable_read();  // run every (_column % 8) == 1 and then the last 4 cycles
	this->attribute_table_read();
	this->process_visible_pixels();
}

void PictureProcessingUnit::render_visible_scanline()
{
	this->_actions.push_back([] { /*noop*/ });
	for (int i = 0; i < 32; ++i)
		this->render_chunk();
	for (int i = 0; i < 8; ++i)
		this->fetch_sprite_data();

	this->_actions.push_back([this] {});
	this->_actions.push_back([this] { this->increment_fine_y_scroll(); });
}

void PictureProcessingUnit::render_visible_scanlines()
{}

void PictureProcessingUnit::render_vblank_scanline()
{}

void PictureProcessingUnit::render_vblank_scanlines()
{}

void PictureProcessingUnit::render_initial_scanline()
{}

ChunkIndices PictureProcessingUnit::compile_pattern_table_bytes()
{
	ChunkIndices indices;

	for (int i = 0; i < 8; ++i)
	{
		uint8_t mask = 0b1000'0000 >> i;
		uint8_t index = ((this->_pattern_table_tile_low_latch & mask) >> (7 - i)) +
			(((this->_pattern_table_tile_high_latch & mask) >> (7 - i)) << 1);
		indices[i] = index;
	}

	return indices;
}

ChunkPixels PictureProcessingUnit::apply_palette_colors(const ChunkIndices& indices)
{
	ChunkPixels pixels;

	for (uint8_t i = 0; i < 8; ++i)
	{
		uint8_t palette_color = 0x3f;
		switch (indices[i])
		{
		case 0:
			palette_color = 0x20;
			break;
		case 1:
			palette_color = 0x10;
			break;
		case 2:
			palette_color = 0x00;
			break;
		case 3:
			palette_color = 0x0f;
			break;
		}
		pixels[i] = _palette->getColor(palette_color);
	}

	return pixels;
}

void PictureProcessingUnit::fetch_sprite_data()
{
	for (int i = 0; i < 8; ++i)
		this->_actions.push_back([this] { this->_vout = 0xFF'00'00; });
}

uint16_t PictureProcessingUnit::calculate_pattern_table_address(bool bit_plane)
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

	uint16_t right_pattern_table_offset = 0b0001'0000'0000'0000;
	uint16_t plane1_offset = 0b0000'00000'0000'1000;
	uint16_t tile_offset = static_cast<uint16_t>(this->_nametable_latch) << 4;
	uint8_t fine_y_scroll_offset =
		static_cast<uint16_t>((this->_vram_address.highByte() & 0b0111'0000) >> 4);

	uint16_t address = tile_offset | fine_y_scroll_offset;
	if (bit_plane)
		address |= plane1_offset;
	if (this->background_pattern_table_select_flag())
		address |= right_pattern_table_offset;

	return address;
}

void PictureProcessingUnit::fetch_sprite_tile_data()
{}

void PictureProcessingUnit::set_fine_x_scroll(uint8_t pos)
{
	this->_fine_x_scroll(pos & 0b00000111);
}

void PictureProcessingUnit::increment_fine_x_scroll()
{
	// Because of the way I am forcing the register to 3 bits, this should effectively work like a
	// mod 8 operation.
	this->_fine_x_scroll(this->_fine_x_scroll() + 1);
}

uint8_t PictureProcessingUnit::get_fine_y_scroll()
{
	return (this->_vram_address.highByte() & 0b0111'0000) >> 4;
}

void PictureProcessingUnit::set_fine_y_scroll(uint8_t pos)
{
	uint8_t scroll_bits = (pos & 0b0000'0111) << 4;
	uint8_t scroll_clear = this->_vram_address.highByte() & 0b0000'1111;
	uint8_t scroll_set = scroll_clear | scroll_bits;
	this->_vram_address.highByte(scroll_set);
}

void PictureProcessingUnit::increment_fine_y_scroll()
{
	// Because of the way I am forcing the most significant bit to 0, this should effectively work
	// like a mod 8 operation.
	this->_vram_address.highByte(this->_vram_address.highByte() + 0b0001'0000);
}

uint8_t PictureProcessingUnit::get_coarse_x_scroll()
{
	return this->_vram_address.lowByte() & 0b0001'1111;
}

void PictureProcessingUnit::set_coarse_x_scroll(const uint8_t pos)
{
	uint8_t scroll_clear = this->_vram_address.lowByte() & 0b1110'0000;
	uint8_t scroll_set = scroll_clear | (pos & 0b0001'1111);
	this->_vram_address.lowByte(scroll_set);
}

void PictureProcessingUnit::increment_coarse_x_scroll()
{
	uint8_t x_scroll = this->_vram_address.lowByte() & 0b0001'1111;
	x_scroll++;
	if ((x_scroll & 0b0010'0000) > 1)
	{
		uint8_t scroll_clear = this->_vram_address() & 0b0111'1111'1110'0000;
		uint8_t nt_toggled = scroll_clear ^ 0b0000'0100'0000'0000;
		this->_vram_address(nt_toggled);
	}
	else
	{
		uint8_t scroll_clear = this->_vram_address.lowByte() & 0b1110'0000;
		uint8_t scroll_set = scroll_clear | (x_scroll & 0b0001'1111);
		this->_vram_address.lowByte(scroll_set);
	}
}

void PictureProcessingUnit::reset_coarse_x_scroll()
{
	uint16_t scroll_bits = this->_temp_vram_address() & 0b0000'0100'0001'1111;
	uint16_t scroll_clear = this->_vram_address() & 0b0111'1011'1110'0000;
	uint16_t scroll_set = scroll_clear | scroll_bits;
	this->_vram_address(scroll_set);
}

uint8_t PictureProcessingUnit::get_coarse_y_scroll()
{
	return static_cast<uint8_t>((this->_vram_address() & 0b0000'0011'1110'0000) >> 5);
}

void PictureProcessingUnit::set_coarse_y_scroll(const uint8_t pos)
{
	uint16_t scroll_bits = static_cast<uint16_t>(pos & 0b0001'1111) << 5;
	uint16_t scroll_clear = this->_vram_address() & 0b0111'1100'0001'1111;
	uint16_t scroll_set = scroll_clear | scroll_bits;
	this->_vram_address(scroll_set);
}

void PictureProcessingUnit::increment_coarse_y_scroll()
{
	uint8_t fine_y = this->get_fine_y_scroll();
	fine_y++;
	this->set_fine_y_scroll(fine_y);
	if ((fine_y & 0b0000'1000) > 0)
	{
		uint8_t coarse_y = this->get_coarse_y_scroll();
		coarse_y++;

		if (coarse_y == 30)
		{
			uint8_t scroll_clear = this->_vram_address() & 0b0000'1000'0001'1111;
			uint8_t nt_toggled = scroll_clear ^ 0b0000'1000'0000'0000;
			this->_vram_address(nt_toggled);
		}
		else if (coarse_y == 31)
		{
			this->set_coarse_y_scroll(0);
		}
		else
		{
			this->set_coarse_y_scroll(coarse_y);
		}
	}
}

void PictureProcessingUnit::reset_coarse_y_scroll()
{
	uint16_t scroll_bits = this->_temp_vram_address() & 0b0111'1011'1110'0000;
	uint16_t scroll_clear = this->_vram_address() & 0b0000'0100'0001'1111;
	uint16_t scroll_set = scroll_clear | scroll_bits;
	this->_vram_address(scroll_set);
}

void PictureProcessingUnit::increment_vram_address()
{
	if (this->increment_mode_flag())
		this->_vram_address(this->_vram_address() + 32);
	else
		this->_vram_address(this->_vram_address() + 1);
}

void PictureProcessingUnit::increment_tile_offset()
{
	uint16_t vram_const_bits = this->_vram_address() & 0b0001'1100'0000'0000;
	uint16_t offset = this->_vram_address() & 0b0000'1111'1111'1111;
	offset = (offset + 1) & 0b0000'1111'1111'1111;
	this->_vram_address(vram_const_bits + offset);
}

uint16_t PictureProcessingUnit::get_nametable_address()
{
	uint16_t nametable_offset = this->_vram_address() & 0b0000'1111'1111'1111;
	return 0x2000 | nametable_offset;
}

uint16_t PictureProcessingUnit::get_attribute_table_address()
{
	// NN 1111 YYY XXX
	// || |||| ||| +++-- high 3 bits of coarse X (x/4)
	// || |||| +++------ high 3 bits of coarse Y (y/4)
	// || ++++---------- attribute offset (960 bytes)
	// ++--------------- nametable select

	uint16_t coarse_x_offset = (this->_vram_address() & 0b0000'0000'0001'1100) >> 2;
	uint16_t coarse_y_offset = (this->_vram_address() & 0b0000'0011'1000'0000) >> 4;
	uint16_t nametable_offset = (this->_vram_address() & 0b0000'1100'0000'0000) << 2;
	uint16_t attribute_table_offset = 0b0011'1100'0000;
	return nametable_offset | attribute_table_offset | coarse_y_offset | coarse_x_offset;
}
