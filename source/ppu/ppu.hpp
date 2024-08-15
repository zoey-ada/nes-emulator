#pragma once

#include <array>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>

#include <cpu/cpu.hpp>

#include "palette.hpp"

using Action = std::function<void()>;
using Register = uint8_t;
using ChunkIndices = std::array<uint8_t, 8>;
using ChunkPixels = std::array<uint32_t, 8>;

class IMemory;

const uint64_t ppu_startup_delay = 88974;

class PictureProcessingUnit
{
public:
	PictureProcessingUnit() = delete;
	PictureProcessingUnit(IMemory* memory);
	virtual ~PictureProcessingUnit();

	void init(Cpu* cpu);

	void clear_registers();
	void power_up();

	void cycle();
	void cycle(uint8_t number_cycles);

	//--------------------------------------------------------------------------
	// pins
	//--------------------------------------------------------------------------
	uint16_t ppu_address_bus() const { return (uint16_t&)this->_ppu_addr[0]; }
	void ppu_address_bus(const uint16_t data) { (uint16_t&)this->_ppu_addr[0] = (data & 0x2fff); }
	// bytes are reversed so that reading the 16 bit values is correct
	uint8_t ppu_address_bus_high() const { return this->_ppu_addr[1]; }
	void ppu_address_bus_high(const uint8_t value) { this->_ppu_addr[1] = (value & 0b00111111); }
	uint8_t ppu_address_bus_low() const { return this->_ppu_addr[0]; }
	void ppu_address_bus_low(const uint8_t value) { this->_ppu_addr[0] = value; }

	uint8_t _ppu_data_bus {0x00};
	uint8_t& ppu_data_bus() { return _ppu_data_bus; }
	// uint8_t& ppu_data_bus() { return this->_ppu_addr[0]; }
	inline uint32_t vout() const { return this->_vout; }

	// registers
	void ppu_ctrl(const uint8_t value);
	inline void nmi_enable_flag(const bool value)
	{
		value ? this->_ppuctrl |= 0b10000000 : this->_ppuctrl &= 0b01111111;
	};
	inline void ppu_master_slave_flag(const bool value)
	{
		value ? this->_ppuctrl |= 0b10000000 : this->_ppuctrl &= 0b10111111;
	};
	inline void sprite_height_flag(const bool value)
	{
		value ? this->_ppuctrl |= 0b00100000 : this->_ppuctrl &= 0b11011111;
	};
	inline void background_pattern_table_select_flag(const bool value)
	{
		value ? this->_ppuctrl |= 0b00100000 : this->_ppuctrl &= 0b11101111;
	};
	inline void sprite_tile_select_flag(const bool value)
	{
		value ? this->_ppuctrl |= 0b00001000 : this->_ppuctrl &= 0b11110111;
	};
	inline void increment_mode_flag(const bool value)
	{
		value ? this->_ppuctrl |= 0b00000100 : this->_ppuctrl &= 0b11111011;
	};
	inline void nametable_select(const uint8_t value)
	{
		switch (value)
		{
		case 0:
			this->_ppuctrl &= 0b11111100;
			break;
		case 1:
			this->_ppuctrl |= 0b00000001;
			this->_ppuctrl &= 0b11111101;
			break;
		case 2:
			this->_ppuctrl |= 0b00000010;
			this->_ppuctrl &= 0b11111110;
			break;
		case 3:
			this->_ppuctrl |= 0b00000011;
			break;
		}
	};

	inline void ppu_mask(const uint8_t value) { this->_ppumask = value; }
	inline void emphasize_blue_flag(const bool value)
	{
		value ? this->_ppumask |= 0b10000000 : this->_ppumask &= 0b01111111;
	};
	inline void emphasize_green_flag(const bool value)
	{
		value ? this->_ppumask |= 0b10000000 : this->_ppumask &= 0b10111111;
	};
	inline void emphasize_red_flag(const bool value)
	{
		value ? this->_ppumask |= 0b00100000 : this->_ppumask &= 0b11011111;
	};
	inline void show_sprites_flag(const bool value)
	{
		value ? this->_ppumask |= 0b00100000 : this->_ppumask &= 0b11101111;
	};
	inline void show_background_flag(const bool value)
	{
		value ? this->_ppumask |= 0b00001000 : this->_ppumask &= 0b11110111;
	};
	inline void show_leftmost_sprites_flag(const bool value)
	{
		value ? this->_ppumask |= 0b00000100 : this->_ppumask &= 0b11111011;
	};
	inline void show_leftmost_background_flag(const bool value)
	{
		value ? this->_ppumask |= 0b00000010 : this->_ppumask &= 0b11111101;
	};
	inline void greyscale_flag(const bool value)
	{
		value ? this->_ppumask |= 0b00000001 : this->_ppumask &= 0b11111110;
	};

	uint8_t ppu_status();
	inline uint8_t vertical_blank_flag() const { return (this->_ppustatus & 0b10000000) > 0; };
	inline uint8_t sprite_zero_hit_flag() const { return (this->_ppustatus & 0b01000000) > 0; };
	inline uint8_t sprite_overflow_flag() const { return (this->_ppustatus & 0b00100000) > 0; };

	inline void oam_addr(const uint8_t value) { this->_oamaddr = value; }

	inline uint8_t oam_data() const { return this->_oamdata; }
	inline void oam_data(const uint8_t value) { this->_oamdata = value; }

	void ppu_scroll(const uint8_t value);
	void ppu_addr(const uint8_t value);
	uint8_t ppu_data();
	void ppu_data(const uint8_t value);

	inline void oam_dma(const uint8_t value) { this->_oamdma = value; }

private:
	Register _ppuctrl {0x00};
	Register _ppumask {0x00};
	Register _ppustatus {0x00};
	Register _oamaddr {0x00};
	Register _oamdata {0x00};
	Register _ppuscroll {0x00};
	std::array<uint8_t, 2> _ppuaddr {0x00, 0x00};
	Register _ppudata {0x00};
	Register _oamdma {0x00};

	// background
	std::array<uint8_t, 2> _vram_address {0x00, 0x00};       // v (15 bits)
	std::array<uint8_t, 2> _temp_vram_address {0x00, 0x00};  // t (15 bits)
	Register _fine_x_scroll {0x00};                          // x (3 bits)
	bool _second_write_flag {false};

	Register _nametable_latch {0x00};
	Register _attribute_table_latch {0x00};
	Register _pattern_table_tile_low_latch {0x00};
	Register _pattern_table_tile_high_latch {0x00};

	Register _address_low_latch {0x00};

	Cpu* _cpu {nullptr};
	std::unique_ptr<Palette> _palette {nullptr};

	// 13 bits
	std::array<uint8_t, 2> _ppu_addr = {0x00, 0x00};

	// argb format (a is ignored)
	uint32_t _vout {0x00000000};
	ChunkPixels _vout_shift_register;
	uint8_t _vout_shift_register_index = 0;

	std::deque<Action> _actions;

	// $0000-$0FFF	$1000	Pattern table 0			Cartridge
	// $1000-$1FFF	$1000	Pattern table 1			Cartridge
	// $2000-$23FF	$0400	Nametable 0				Cartridge
	// $2400-$27FF	$0400	Nametable 1				Cartridge
	// $2800-$2BFF	$0400	Nametable 2				Cartridge
	// $2C00-$2FFF	$0400	Nametable 3				Cartridge
	// $3000-$3EFF	$0F00	Mirrors of $2000-$2EFF	Cartridge
	// $3F00-$3F1F	$0020	Palette RAM indexes		Internal
	// $3F20-$3FFF	$00E0	Mirrors of $3F00-$3F1F	Internal
	IMemory* _memory {nullptr};

	uint16_t _tile_1 {0x0000};
	uint16_t _tile_2 {0x0000};

	uint8_t _palette_attributes_1 {0x00};
	uint8_t _palette_attributes_2 {0x00};

	// sprites
	IMemory* _object_attribute_memory {nullptr};            // 64 (sprites) * 4 (sprite size) bytes
	IMemory* _secondary_object_attribute_memory {nullptr};  // 8 * 4 bytes

	uint8_t sprite_0_pattern_table_data {0x00};
	uint8_t sprite_1_pattern_table_data {0x00};
	uint8_t sprite_2_pattern_table_data {0x00};
	uint8_t sprite_3_pattern_table_data {0x00};
	uint8_t sprite_4_pattern_table_data {0x00};
	uint8_t sprite_5_pattern_table_data {0x00};
	uint8_t sprite_6_pattern_table_data {0x00};
	uint8_t sprite_7_pattern_table_data {0x00};

	uint8_t sprite_0_attributes {0x00};
	uint8_t sprite_1_attributes {0x00};
	uint8_t sprite_2_attributes {0x00};
	uint8_t sprite_3_attributes {0x00};
	uint8_t sprite_4_attributes {0x00};
	uint8_t sprite_5_attributes {0x00};
	uint8_t sprite_6_attributes {0x00};
	uint8_t sprite_7_attributes {0x00};

	uint8_t sprite_0_x_position {0x00};
	uint8_t sprite_1_x_position {0x00};
	uint8_t sprite_2_x_position {0x00};
	uint8_t sprite_3_x_position {0x00};
	uint8_t sprite_4_x_position {0x00};
	uint8_t sprite_5_x_position {0x00};
	uint8_t sprite_6_x_position {0x00};
	uint8_t sprite_7_x_position {0x00};

	uint16_t _row {0};
	uint16_t _column {0};
	bool _is_even_frame {false};

	bool _is_starting_up {true};
	uint64_t _startup_delay {ppu_startup_delay};

	//--------------------------------------------------------------------------
	// pins
	//--------------------------------------------------------------------------
	inline bool cpu_read_write() { return this->_cpu->read_write(); }
	inline uint8_t& cpu_data() { return this->_cpu->data_bus(); }
	inline uint8_t cpu_addr() { return (this->_cpu->address_bus_low() & 0b0000'0111); }

	//--------------------------------------------------------------------------
	// registers
	//--------------------------------------------------------------------------
	uint16_t vram_address() const { return (uint16_t&)this->_vram_address[0]; }
	void vram_address(const uint16_t data) { (uint16_t&)this->_vram_address[0] = (data & 0x7fff); }
	// bytes are reversed so that reading the 16 bit values is correct
	uint8_t vram_address_high() const { return this->_vram_address[1]; }
	void vram_address_high(const uint8_t value) { this->_vram_address[1] = (value & 0b0111'1111); }
	uint8_t vram_address_low() const { return this->_vram_address[0]; }
	void vram_address_low(const uint8_t value) { this->_vram_address[0] = value; }

	uint16_t temp_vram_address() const { return (uint16_t&)this->_temp_vram_address[0]; }
	void temp_vram_address(const uint16_t data)
	{
		(uint16_t&)this->_temp_vram_address[0] = (data & 0x4fff);
	}
	// bytes are reversed so that reading the 16 bit values is correct
	uint8_t temp_vram_address_high() const { return this->_temp_vram_address[1]; }
	void temp_vram_address_high(const uint8_t value)
	{
		this->_temp_vram_address[1] = (value & 0b0111'1111);
	}
	uint8_t temp_vram_address_low() const { return this->_temp_vram_address[0]; }
	void temp_vram_address_low(const uint8_t value) { this->_temp_vram_address[0] = value; }

	uint8_t fine_x_scroll() const { return this->_fine_x_scroll; }
	void fine_x_scroll(const uint8_t value) { this->_fine_x_scroll = (value & 0b0000'0111); }

	//--------------------------------------------------------------------------
	// PPUCTRL
	//--------------------------------------------------------------------------
	inline bool nmi_enable_flag() const { return (this->_ppuctrl & 0b1000'0000) > 0; }
	inline bool ppu_master_slave_flag() const { return (this->_ppuctrl & 0b0100'0000) > 0; }
	inline bool sprite_height_flag() const { return (this->_ppuctrl & 0b0010'0000) > 0; }
	inline bool background_pattern_table_select_flag() const
	{
		return (this->_ppuctrl & 0b0001'0000) > 0;
	}
	inline bool sprite_tile_select_flag() const { return (this->_ppuctrl & 0b0000'1000) > 0; }
	inline bool increment_mode_flag() const { return (this->_ppuctrl & 0b0000'0100) > 0; }
	inline uint8_t nametable_select() const { return this->_ppuctrl & 0b0000'0011; };

	//--------------------------------------------------------------------------
	// PPUSTATUS
	//--------------------------------------------------------------------------
	inline void vertical_blank_flag(const bool value)
	{
		this->_ppustatus = value ? this->_ppustatus | 0b1000'0000 : this->_ppustatus & 0b0111'1111;
	};
	inline void sprite_zero_hit_flag(const bool value)
	{
		value ? this->_ppustatus |= 0b0100'0000 : this->_ppustatus &= 0b1011'1111;
	};
	inline void sprite_overflow_flag(const bool value)
	{
		value ? this->_ppustatus |= 0b0010'1000 : this->_ppustatus &= 0b1101'1111;
	};

	//--------------------------------------------------------------------------
	// PPUMASK
	//--------------------------------------------------------------------------
	inline bool emphasize_blue_flag() const { return (this->_ppumask & 0b10000000) > 0; };
	inline bool emphasize_green_flag() const { return (this->_ppumask & 0b10000000) > 0; };
	inline bool emphasize_red_flag() const { return (this->_ppumask & 0b00100000) > 0; };
	inline bool show_sprites_flag() const { return (this->_ppumask & 0b00100000) > 0; };
	inline bool show_background_flag() const { return (this->_ppumask & 0b00001000) > 0; };
	inline bool show_leftmost_sprites_flag() const { return (this->_ppumask & 0b00000100) > 0; };
	inline bool show_leftmost_background_flag() const { return (this->_ppumask & 0b00000010) > 0; };
	inline bool greyscale_flag() const { return (this->_ppumask & 0b00000001) > 0; };

	//--------------------------------------------------------------------------
	void load_next_operation();
	void read_memory();
	void write_memory();

	void feed_vout();

	void nametable_read();
	void attribute_table_read();
	void process_visible_pixels();
	void render_chunk();
	void render_visible_scanline();
	void render_visible_scanlines();
	void render_vblank_scanline();
	void render_vblank_scanlines();
	void render_initial_scanline();

	ChunkIndices compile_pattern_table_bytes();
	ChunkPixels apply_palette_colors(const ChunkIndices& indices);
	void fetch_sprite_data();

	uint16_t calculate_pattern_table_address(bool bit_plane);
	void fetch_sprite_tile_data();

	void set_fine_x_scroll(const uint8_t pos);
	void increment_fine_x_scroll();

	uint8_t get_fine_y_scroll();
	void set_fine_y_scroll(const uint8_t pos);
	void increment_fine_y_scroll();

	uint8_t get_coarse_x_scroll();
	void set_coarse_x_scroll(const uint8_t pos);
	void increment_coarse_x_scroll();
	void reset_coarse_x_scroll();

	uint8_t get_coarse_y_scroll();
	void set_coarse_y_scroll(const uint8_t pos);
	void increment_coarse_y_scroll();
	void reset_coarse_y_scroll();

	void increment_vram_address();
	void increment_tile_offset();
	uint16_t get_nametable_address();
	uint16_t get_attribute_table_address();
};
