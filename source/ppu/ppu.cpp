#include "ppu.hpp"

#include <cassert>
#include <cmath>
#include <optional>

#include <base/iCpu.hpp>
#include <base/iMemory.hpp>
#include <base/randomAccessMemory.hpp>
#include <base/slowMemory.hpp>

PictureProcessingUnit::PictureProcessingUnit(IMemory* memory, IMemory* oam)
	: _memory(memory), _object_attribute_memory(oam)
{
	assert(this->_memory);
	assert(this->_object_attribute_memory);

	this->_secondary_object_attribute_memory = std::make_unique<SlowMemory>(0x20);
	this->_sys_palette = std::make_unique<SystemPalette>();
}

PictureProcessingUnit::~PictureProcessingUnit()
{
	this->_cpu = nullptr;
	this->_memory = nullptr;
	this->_object_attribute_memory = nullptr;
}

void PictureProcessingUnit::init(ICpu* cpu)
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

	if (this->rendering_enabled())
	{
		if (this->_background_actions.empty())
			this->load_next_background_operation();
		if (!this->_background_actions.empty())
		{
			auto background_action = this->_background_actions.front();
			this->_background_actions.pop_front();
			background_action();
		}

		if (this->_sprite_actions.empty())
			this->load_next_sprite_operation();
		if (!this->_sprite_actions.empty())
		{
			auto sprite_action = this->_sprite_actions.front();
			this->_sprite_actions.pop_front();
			sprite_action();
		}
	}
	else
	{
		this->_background_actions.clear();
		this->_sprite_actions.clear();
	}

	if (this->_actions.empty())
		this->load_next_operation();

	if (!this->_actions.empty())
	{
		auto action = this->_actions.front();
		this->_actions.pop_front();
		action();
	}

	// auto action = this->_actions.front();
	// this->_actions.pop_front();
	// action();

	if (this->_row < 240)
	{
		if (this->_column < 256)
			if (this->rendering_enabled())
				this->feed_vout();
			else
				this->_vout = 0x000000;
		else
			this->_vout = 0xff0000;
	}
	else
	{
		this->_vout = 0x00ff00;
	}

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

void PictureProcessingUnit::reset()
{
	this->_actions.clear();
	this->_row = 0;
	this->_column = 0;
	this->_is_even_frame = false;
	this->_vram_address(0x0000);
	this->_temp_vram_address(0x0000);
}

void PictureProcessingUnit::cycle(uint64_t number_of_cycles)
{
	for (auto i = 0; i < number_of_cycles; ++i)
		this->cycle();
}

void PictureProcessingUnit::cpu_read()
{
	switch (this->_cpu_address_bus())
	{
	case 0:  // PPUCTRL
		break;
	case 1:  // PPUMASK
		break;
	case 2:  // PPUSTATUS
		this->_cpu_data_bus(this->ppu_status());
		break;
	case 3:  // OAMADDR
		break;
	case 4:  // OAMDATA
		this->_cpu_data_bus(this->oam_data());
		break;
	case 5:  // PPUSCROLL
		break;
	case 6:  // PPUADDR
		break;
	case 7:  // PPUDATA
		this->_cpu_data_bus(this->ppu_data());
		break;
	}
}

void PictureProcessingUnit::cpu_write()
{
	switch (this->_cpu_address_bus())
	{
	case 0:  // PPUCTRL
		this->ppu_ctrl(this->_cpu_data_bus());
		break;
	case 1:  // PPUMASK
		this->ppu_mask(this->_cpu_data_bus());
		break;
	case 2:  // PPUSTATUS
		break;
	case 3:  // OAMADDR
		this->oam_addr(this->_cpu_data_bus());
		break;
	case 4:  // OAMDATA
		this->oam_data(this->_cpu_data_bus());
		break;
	case 5:  // PPUSCROLL
		this->ppu_scroll(this->_cpu_data_bus());
		break;
	case 6:  // PPUADDR
		this->ppu_addr(this->_cpu_data_bus());
		break;
	case 7:  // PPUDATA
		this->ppu_data(this->_cpu_data_bus());
		break;
	}
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

inline uint8_t PictureProcessingUnit::oam_data() const
{
	return this->read_oam();
}

inline void PictureProcessingUnit::oam_data(const uint8_t value)
{
	if ((this->_row < 240 || this->_row == 261) && this->rendering_enabled())
	{
		// no write
		this->_oam_address(this->_oam_address() + 0b0000'0100);
		return;
	}

	this->_object_attribute_memory->write(this->_oam_address(), value);
	++this->_oam_address;
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
	uint8_t return_data = this->_ppu_data_read_buffer();
	this->_address_bus(this->_vram_address());
	this->read_memory();

	if (this->_address_bus() >= 0x3f00)
		return_data = this->_data_bus();

	this->increment_vram_address();
	this->_ppu_data_read_buffer(this->_data_bus());

	return return_data;
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
	if (this->_row < 240 && this->rendering_enabled())
	{
		if (this->_column >= 2)
		{
			this->sprite_zero_hit_flag(this->_sprite_0_hit);

			if (this->_column % 8 == 0)
				this->update_vout_registers();
		}
	}

	if (this->_row == 241 && this->_column == 1)
	{
		this->_actions.push_back([this] {
			this->vertical_blank_flag(true);

			if (this->_ppuctrl.nmi_enable_flag())
				this->_cpu->nmi();
		});
	}
	else if (this->_row == 261)
	{
		if (this->_column == 1)
		{
			this->_actions.push_back([this] {
				this->vertical_blank_flag(false);
				this->_sprite_0_hit = false;
				this->sprite_zero_hit_flag(false);
				this->sprite_overflow_flag(false);
			});
		}
		else if (this->rendering_enabled() && (this->_column == 328 || this->_column == 336))
		{
			this->update_vout_registers();
		}
	}
}

void PictureProcessingUnit::load_next_background_operation()
{
	if (this->_row > 239 && this->_row < 261)
		// in vblank; nothing to do
		return;

	if (this->_column >= 1 && this->_column < 257)
	{
		if (this->_column % 8 == 1)
			this->render_chunk();
	}
	else if (this->_row != 261 && this->_column == 257)
	{
		this->_actions.push_back([this] { this->increment_coarse_y_scroll(); });
	}
	else if (this->_row == 261 && (this->_column >= 280 && this->_column <= 304))
	{
		this->_actions.push_back([this] { this->reset_coarse_y_scroll(); });
	}
	else if (this->_column >= 321 && this->_column < 337)
	{
		if (this->_column % 8 == 1)
			this->render_chunk();
	}
	else if (this->_column >= 337 && this->_column <= 340)
	{
		this->nametable_read();
	}
}

void PictureProcessingUnit::load_next_sprite_operation()
{
	if (this->_row > 239)
		// in vblank; nothing to do
		return;

	if (this->_column == 0 || this->_column >= 321)
	{  // repeatedly read secondary oram[0] for no reason
		this->_sprite_actions.push_back([this] { this->read_secondary_oam(); });
	}
	else if (this->_column == 1)
		this->clear_secondary_oam();
	else if (this->_column < 257)
		this->evaluate_sprites();
	else  // this->_column < 321
	{
		// get rendering data, x pos, and attributes for the 8 sprites detected
		if (this->_column % 8 == 1)
			this->load_sprite();
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

uint8_t PictureProcessingUnit::read_oam() const
{
	if (this->_oam_force_ff_read)
		return 0xff;
	else
		return this->_object_attribute_memory->read(this->_oam_address());
}

uint8_t PictureProcessingUnit::read_secondary_oam() const
{
	return this->_secondary_object_attribute_memory->read(this->_secondary_oam_address());
}

void PictureProcessingUnit::write_secondary_oam(const uint8_t data)
{
	if (!this->_secondary_oam_readonly)
		this->_secondary_object_attribute_memory->write(this->_secondary_oam_address(), data);
}

void PictureProcessingUnit::feed_vout()
{
	this->_vout =
		this->_vout_shift_register[this->_vout_shift_register_index + this->_fine_x_scroll()];
	this->_vout_shift_register_index = (this->_vout_shift_register_index + 1) % 8;
}

void PictureProcessingUnit::nametable_read()
{
	this->_background_actions.push_back(
		[this] { this->_address_bus(this->get_nametable_address()); });
	this->_background_actions.push_back([this] {
		this->read_memory();
		this->_nametable_latch = this->_data_bus();
	});
}

void PictureProcessingUnit::attribute_table_read()
{
	this->_background_actions.push_back(
		[this] { this->_address_bus(this->get_attribute_table_address()); });
	this->_background_actions.push_back([this] {
		this->read_memory();
		this->_attribute_table_latch = this->_data_bus();
	});
}

void PictureProcessingUnit::process_visible_pixels()
{
	this->_background_actions.push_back(
		[this] { this->_address_bus(this->calculate_pattern_table_address(false)); });
	this->_background_actions.push_back([this] {
		this->read_memory();
		this->_pattern_table_tile_low_latch = this->_data_bus();
	});

	this->_background_actions.push_back(
		[this] { this->_address_bus(this->calculate_pattern_table_address(true)); });
	this->_background_actions.push_back([this] {
		this->read_memory();
		this->_pattern_table_tile_high_latch = this->_data_bus();

		if (this->_column == 256)
			this->reset_coarse_x_scroll();
		else
			this->increment_coarse_x_scroll();

		this->_background_index_latch = std::move(this->compile_pattern_table_bytes());
	});
}

void PictureProcessingUnit::render_chunk()
{
	this->nametable_read();  // run every (_column % 8) == 1 and then the last 4 cycles
	this->attribute_table_read();
	this->process_visible_pixels();
}

void PictureProcessingUnit::clear_secondary_oam()
{
	for (uint8_t i = 0; i < 64; ++i)
	{
		if (i == 0)
		{
			this->_sprite_actions.push_back([this] {
				this->_secondary_oam_readonly = false;
				this->_secondary_oam_address(0x00);
				this->_oam_force_ff_read = true;
				this->write_secondary_oam(this->read_oam());
				++this->_secondary_oam_address;
			});
		}
		else if (i == 63)
		{
			this->_sprite_actions.push_back([this] {
				this->write_secondary_oam(this->read_oam());
				this->_oam_force_ff_read = false;
				this->_oam_address(0x00);
				++this->_secondary_oam_address;
			});
		}
		else
		{
			this->_sprite_actions.push_back([this] {
				this->write_secondary_oam(this->read_oam());
				++this->_secondary_oam_address;
			});
		}
	}
}

void PictureProcessingUnit::evaluate_sprites()
{
	// copy y pos and evaluate if sprite is in next line
	this->_sprite_actions.push_back([this] { this->_sprite_data_latch = this->read_oam(); });
	this->_sprite_actions.push_back([this] {
		auto& sprite_y = this->_sprite_data_latch;
		this->write_secondary_oam(sprite_y);

		auto sprite_height = this->_ppuctrl.sprite_height_flag() ? 16 : 8;
		auto sprite_bottom_y = sprite_y + sprite_height;
		if (this->_row >= sprite_y && this->_row < sprite_bottom_y)
		{
			if (this->_secondary_oam_readonly)
				// found sprite 9+
				this->sprite_overflow_flag(true);

			// if this is sprite 9+ the data is not actually copied over because secondary oam is
			// set to readonly
			++this->_oam_address;
			++this->_secondary_oam_address;
			this->copy_sprite_data();
		}
		else
		{
			this->_oam_address(this->_oam_address() + 4);

			// emulate hardware bug
			if (this->_secondary_oam_readonly)
				++this->_oam_address;
		}
	});
}

void PictureProcessingUnit::copy_sprite_data()
{
	// copy tile index
	this->_sprite_actions.push_back([this] { this->_sprite_data_latch = this->read_oam(); });
	this->_sprite_actions.push_back([this] {
		this->write_secondary_oam(this->_sprite_data_latch);
		this->_oam_address(this->_oam_address() + 1);
		this->_secondary_oam_address(this->_secondary_oam_address() + 1);
	});

	// copy attributes
	this->_sprite_actions.push_back([this] { this->_sprite_data_latch = this->read_oam(); });
	this->_sprite_actions.push_back([this] {
		this->write_secondary_oam(this->_sprite_data_latch);
		this->_oam_address(this->_oam_address() + 1);
		this->_secondary_oam_address(this->_secondary_oam_address() + 1);
	});

	// copy x pos
	this->_sprite_actions.push_back([this] { this->_sprite_data_latch = this->read_oam(); });
	this->_sprite_actions.push_back([this] {
		this->write_secondary_oam(this->_sprite_data_latch);
		this->_oam_address(this->_oam_address() + 1);
		this->_secondary_oam_address(this->_secondary_oam_address() + 1);

		if (this->_secondary_oam_address() == 0x41)
		{
			// 8 sprites have been found
			this->_secondary_oam_readonly = true;
		}
	});
}

void PictureProcessingUnit::load_sprite()
{
	this->_actions.push_back([this] {
		// this->_address_bus(this->get_nametable_address());
		this->_sprite_y_pos_latch = this->read_secondary_oam();
		++this->_secondary_oam_address;
	});
	this->_actions.push_back([this] {
		// this->read_memory();
		// this->_nametable_latch = this->_data_bus();
		this->_sprite_pt_index_latch = this->read_secondary_oam();
		++this->_secondary_oam_address;
	});

	this->_actions.push_back([this] {
		// this->_address_bus(this->get_attribute_table_address());
		uint8_t sprite_index = this->get_sprite_index();
		this->_sprite_attribute_latches[sprite_index] = this->read_secondary_oam();
		++this->_secondary_oam_address;
	});
	this->_actions.push_back([this] {
		// this->read_memory();
		// this->_attribute_table_latch = this->_data_bus();
		uint8_t sprite_index = this->get_sprite_index();
		this->_sprite_x_pos_latches[sprite_index] = this->read_secondary_oam();
		++this->_secondary_oam_address;
	});

	this->_actions.push_back(
		[this] { this->_address_bus(this->calclutate_sprite_pt_address(false)); });
	this->_actions.push_back([this] {
		this->read_memory();
		this->_sprite_pt_low_latch = this->_data_bus();
	});

	this->_actions.push_back(
		[this] { this->_address_bus(this->calclutate_sprite_pt_address(true)); });
	this->_actions.push_back([this] {
		this->read_memory();
		this->_sprite_pt_high_latch = this->_data_bus();

		uint8_t sprite_index = this->get_prev_sprite_index();
		this->_sprite_index_latches[sprite_index] = std::move(this->compile_sprite_pt_bytes());
	});
}

ChunkIndices PictureProcessingUnit::compile_pattern_table_bytes() const
{
	ChunkIndices indices;

	for (int i = 0; i < 8; ++i)
	{
		uint8_t mask = 0b1000'0000 >> i;
		uint8_t index = ((this->_pattern_table_tile_low_latch & mask) >> (7 - i)) |
			(((this->_pattern_table_tile_high_latch & mask) >> (7 - i)) << 1);
		indices[i] = index;
	}

	return indices;
}

ChunkIndices PictureProcessingUnit::compile_sprite_pt_bytes() const
{
	uint8_t sprite_index = this->get_prev_sprite_index();
	bool horizontal_flip = (this->_sprite_attribute_latches[sprite_index] & 0b0100'0000) > 0;
	ChunkIndices indices;

	for (int i = 0; i < 8; ++i)
	{
		uint8_t mask = 0b1000'0000 >> i;
		uint8_t index = ((this->_pattern_table_tile_low_latch & mask) >> (7 - i)) +
			(((this->_pattern_table_tile_high_latch & mask) >> (7 - i)) << 1);

		if (horizontal_flip)
			indices[7 - i] = index;
		else
			indices[i] = index;
	}

	return indices;
}

uint8_t PictureProcessingUnit::parse_background_palette_number() const
{
	bool x = (this->_vram_address.lowByte() & 0b0000'0010) > 1;
	bool y = (this->_vram_address.lowByte() & 0b0100'0000) > 1;

	uint8_t palette_num = this->_attribute_table_latch;
	if (x)
		palette_num = palette_num >> 2;
	if (y)
		palette_num = palette_num >> 4;
	return palette_num & 0b0000'0011;
}

uint8_t PictureProcessingUnit::parse_sprite_palette_number() const
{
	uint8_t sprite_index = this->get_prev_sprite_index();
	return this->_sprite_attribute_latches[sprite_index] & 0b0100'0011;
}

uint32_t PictureProcessingUnit::apply_palette_color(const uint8_t& index,
	const uint8_t palette_number) const
{
	uint16_t palette_address = 0x3f00;

	if (index == 0)
		return this->_memory->read(palette_address);
	else
	{
		uint16_t palette_offset = palette_number << 2;
		return this->_memory->read(palette_address + palette_offset + index);
	}
}

ChunkPixels PictureProcessingUnit::apply_palette_colors(const ChunkIndices& indices,
	const uint8_t palette_number) const
{
	uint16_t palette_offset = palette_number << 2;

	uint8_t color0 = this->_memory->read(0x3f00);
	uint8_t color1 = this->_memory->read(0x3f00 + palette_offset + 1);
	uint8_t color2 = this->_memory->read(0x3f00 + palette_offset + 2);
	uint8_t color3 = this->_memory->read(0x3f00 + palette_offset + 3);

	ChunkPixels pixels;

	for (uint8_t i = 0; i < 8; ++i)
	{
		uint8_t palette_color = 0x3f;
		switch (indices[i])
		{
		case 0:
			palette_color = color0;
			break;
		case 1:
			palette_color = color1;
			break;
		case 2:
			palette_color = color2;
			break;
		case 3:
			palette_color = color3;
			break;
		}
		pixels[i] = _sys_palette->getColor(palette_color);
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
	if (this->_ppuctrl.background_pattern_table_select_flag())
		address |= right_pattern_table_offset;

	return address;
}

uint16_t PictureProcessingUnit::calclutate_sprite_pt_address(bool bit_plane)
{
	uint16_t right_pattern_table_offset = 0b0001'0000'0000'0000;
	uint16_t plane1_offset = 0b0000'00000'0000'1000;
	uint16_t tile_offset = 0;
	bool is_right_pt = false;

	uint8_t sprite_index = this->get_prev_sprite_index();
	bool vertical_flip = (this->_sprite_attribute_latches[sprite_index] & 0b1000'0000) > 0;
	auto sprite_height = this->_ppuctrl.sprite_height_flag() ? 16 : 8;

	uint8_t fine_y = this->_row - this->_sprite_y_pos_latch;
	if (vertical_flip)
		fine_y = sprite_height - fine_y - 1;

	uint8_t fine_y_scroll_offset = static_cast<uint16_t>(fine_y & 0b0000'0111);

	if (!this->_ppuctrl.sprite_height_flag())
	{
		is_right_pt = this->_ppuctrl.sprite_pattern_table_select_flag();
		tile_offset = static_cast<uint16_t>(this->_sprite_pt_index_latch) << 4;
	}
	else
	{
		is_right_pt = (this->_sprite_pt_index_latch & 0b0000'0001) > 0;
		uint8_t tile_index = (this->_sprite_pt_index_latch & 0b1111'1110) >> 1;

		if (fine_y <= 7)
			tile_offset = static_cast<uint16_t>(tile_index) << 4;
		else
			tile_offset = static_cast<uint16_t>(tile_index + 1) << 4;
	}

	uint16_t address = tile_offset | fine_y_scroll_offset;
	if (bit_plane)
		address |= plane1_offset;
	if (is_right_pt)
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
		uint16_t scroll_clear = this->_vram_address() & 0b0111'1111'1110'0000;
		uint16_t nt_toggled = scroll_clear ^ 0b0000'0100'0000'0000;
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
	if (this->_ppuctrl.increment_mode_flag())
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
	return 0x2000 | nametable_offset | attribute_table_offset | coarse_y_offset | coarse_x_offset;
}

ChunkPixels PictureProcessingUnit::determine_output_pixels()
{
	uint8_t chunk_x_pos = (this->get_coarse_x_scroll() << 3) - 8;
	uint8_t background_palette = this->parse_background_palette_number();
	ChunkPixels pixels;

	for (int i = 0; i < 8; ++i)
	{
		uint8_t background_pixel_index = this->_background_index_latch[i];
		pixels[i] = this->apply_palette_color(background_pixel_index, background_palette);
		uint8_t x_pos = chunk_x_pos + i;

		for (int sprite_index = 0; sprite_index < 8; ++sprite_index)
		{
			uint8_t sprite_attr = this->_sprite_attribute_latches[sprite_index];
			if (sprite_attr == 255)
				break;

			uint8_t sprite_x_pos = this->_sprite_x_pos_latches[sprite_index];
			int16_t x_pos_diff = sprite_x_pos - x_pos;

			if (x_pos_diff <= 0 && x_pos_diff > -8)
			{
				uint8_t sprite_pixel_index = this->_sprite_index_latches[sprite_index][-x_pos_diff];
				bool sprite_in_front =
					(this->_sprite_attribute_latches[sprite_index] & 0b0010'0000) > 0;

				if (sprite_pixel_index == 0)
					continue;

				else
				{
					if (sprite_in_front || background_pixel_index == 0)
					{
						uint8_t sprite_palette =
							(this->_sprite_attribute_latches[sprite_index] & 0b0000'0011) + 3;

						pixels[i] = this->apply_palette_color(sprite_pixel_index, sprite_palette);
					}

					if (!this->_sprite_0_hit && sprite_index == 0 && background_pixel_index > 0)
						this->_sprite_0_hit = true;
				}

				break;
			}
		}
		pixels[i] = _sys_palette->getColor(pixels[i]);
	}

	return pixels;
}

void PictureProcessingUnit::update_vout_registers()
{
	uint32_t* front_eight = this->_vout_shift_register.data();
	uint32_t* back_eight = front_eight + 8;
	memcpy(front_eight, back_eight, 8 * sizeof(uint32_t));
	memcpy(back_eight, this->determine_output_pixels().data(), 8 * sizeof(uint32_t));
	this->_vout_shift_register_index = 0;
}
