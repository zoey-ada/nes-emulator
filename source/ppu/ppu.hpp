#pragma once

#include <cstdint>
#include <deque>
#include <functional>

using Action = std::function<void()>;

class IMemory;

class PictureProcessingUnit
{
public:
	PictureProcessingUnit() = default;
	virtual ~PictureProcessingUnit() = default;

	void cycle();
	void cycle(uint8_t number_cycles);

	// pins
	inline void cpu_data(const uint8_t value) { this->_cpu_bus = value; }
	inline uint8_t cpu_data() const { return this->_cpu_bus; }
	inline void cpu_addr(const uint8_t value) { this->_cpu_addr = value; }
	inline uint8_t cpu_addr() const { return this->_cpu_addr; }
	inline uint32_t vout() const { return this->_vout; }

	// registers
	inline void ppu_ctrl(const uint8_t value) { this->_ppuctrl = value; }
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

	inline uint8_t ppu_status() const { return this->_ppustatus; }
	inline uint8_t vertical_blank_flag(const bool value)
	{
		return (this->_ppustatus &= 0b10000000) > 0;
	};
	inline uint8_t sprite_zero_hit_flag(const bool value)
	{
		return (this->_ppustatus &= 0b10000000) > 0;
	};
	inline uint8_t sprite_overflow_flag(const bool value)
	{
		return (this->_ppustatus &= 0b00100000) > 0;
	};

	inline void oam_addr(const uint8_t value) { this->_oamaddr = value; }

	inline uint8_t oam_data() const { return this->_oamdata; }
	inline void oam_data(const uint8_t value) { this->_oamdata = value; }

	inline void ppu_scroll(const uint8_t value) { this->_ppuscroll = value; }

	inline void ppu_addr(const uint8_t value) { this->_ppuaddr = value; }

	inline uint8_t ppu_data() const { return this->_ppudata; }
	inline void ppu_data(const uint8_t value) { this->_ppudata = value; }

	inline void oam_dma(const uint8_t value) { this->_oamdma = value; }

private:
	uint8_t _ppuctrl {0x00};
	uint8_t _ppumask {0x00};
	uint8_t _ppustatus {0x00};
	uint8_t _oamaddr {0x00};
	uint8_t _oamdata {0x00};
	uint8_t _ppuscroll {0x00};
	uint8_t _ppuaddr {0x00};
	uint8_t _ppudata {0x00};
	uint8_t _oamdma {0x00};

	uint8_t _cpu_bus {0x00};
	// 3 bits
	uint8_t _cpu_addr {0x00};

	// argb format (a is ignored)
	uint32_t _vout {0x00000000};

	std::deque<Action> _actions;

	// $0000-$0FFF	$1000	Pattern table 0
	// $1000-$1FFF	$1000	Pattern table 1
	// $2000-$23FF	$0400	Nametable 0
	// $2400-$27FF	$0400	Nametable 1
	// $2800-$2BFF	$0400	Nametable 2
	// $2C00-$2FFF	$0400	Nametable 3
	// $3000-$3EFF	$0F00	Mirrors of $2000-$2EFF
	// $3F00-$3F1F	$0020	Palette RAM indexes
	// $3F20-$3FFF	$00E0	Mirrors of $3F00-$3F1F
	IMemory* _memory {nullptr};

	// background
	uint16_t _vram_address {0x0000};
	uint16_t _temp_vram_address {0x0000};
	uint16_t _fine_x_scroll {0x0000};
	bool _second_write_flag {false};

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

	uint16_t calculate_pattern_table_address(uint8_t row_of_tile, uint8_t nametable_entry);
	void fetch_background_tile_data(uint8_t current_row);
	void fetch_sprite_tile_data();
};
