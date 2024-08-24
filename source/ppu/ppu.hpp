#pragma once

#include <array>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>

#include <base/iPpu.hpp>
#include <base/register.hpp>

#include "systemPalette.hpp"

using Action = std::function<void()>;
using Register = uint8_t;
using ChunkIndices = std::array<uint8_t, 8>;
using ChunkPixels = std::array<uint32_t, 8>;
using DoubleChunkPixels = std::array<uint32_t, 16>;

class ICpu;
class IMemory;

const uint64_t ppu_startup_delay = 88974;

class PictureProcessingUnit: public IPpu
{
public:
	explicit PictureProcessingUnit(IMemory* memory);
	virtual ~PictureProcessingUnit();

	void init(ICpu* cpu) override;

	void reset() override;

	void cycle() override;
	void cycle(uint64_t number_of_cycles) override;

	inline uint16_t address_bus() const override { return this->_address_bus(); }
	inline void address_bus(const uint16_t data) override { this->_address_bus(data); }
	inline uint8_t data_bus() const override { return this->_data_bus(); }
	inline void data_bus(const uint8_t data) override { this->_data_bus(data); }

	inline void cpu_address_bus(const uint8_t data) override { this->_cpu_address_bus(data); }
	inline uint8_t cpu_data_bus() const override { return this->_cpu_data_bus(); }
	inline void cpu_data_bus(const uint8_t data) override { this->_cpu_data_bus(data); }

	void cpu_read() override;
	void cpu_write() override;

	bool read_write() const override { return this->_read_write; }
	inline uint32_t vout() const override { return this->_vout; }

	// registers
	void ppu_ctrl(const uint8_t value);
	inline void ppu_mask(const uint8_t value) { this->_ppumask(value); }
	uint8_t ppu_status();
	inline void oam_addr(const uint8_t value) { this->_oamaddr(value); }
	inline uint8_t oam_data() const { return this->_oamdata(); }
	inline void oam_data(const uint8_t value) { this->_oamdata(value); }
	void ppu_scroll(const uint8_t value);
	void ppu_addr(const uint8_t value);
	uint8_t ppu_data();
	void ppu_data(const uint8_t value);
	inline void oam_dma(const uint8_t value) override { this->_oamdma(value); }

protected:
	IMemory* _memory {nullptr};
	std::unique_ptr<SystemPalette> _sys_palette {nullptr};

	Register_14bit _address_bus;
	// TODO: technically this is supposed to be the lower byte of the address bus
	Register_8bit _data_bus;

	Register_3bit _cpu_address_bus;
	Register_8bit _cpu_data_bus;

	virtual void write_memory();

private:
	Register_8bit _ppuctrl;
	Register_8bit _ppumask;
	Register_8bit _ppustatus;
	Register_8bit _oamaddr;
	Register_8bit _oamdata;
	Register_8bit _oamdma;
	Register_8bit _ppu_data_read_buffer;

	Register_15bit _vram_address;
	Register_15bit _temp_vram_address;
	Register_3bit _fine_x_scroll;
	bool _second_write_flag {false};

	Register _nametable_latch {0x00};
	Register _attribute_table_latch {0x00};
	Register _pattern_table_tile_low_latch {0x00};
	Register _pattern_table_tile_high_latch {0x00};

	bool _read_write {false};

	ICpu* _cpu {nullptr};

	// argb format (a is ignored)
	uint32_t _vout {0x00000000};
	ChunkPixels _vout_latch;
	DoubleChunkPixels _vout_shift_register;
	uint8_t _vout_shift_register_index = 0;

	std::deque<Action> _actions;

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
	// PPUCTRL
	//--------------------------------------------------------------------------
	inline bool nmi_enable_flag() const { return (this->_ppuctrl() & 0b1000'0000) > 0; }
	inline bool ppu_master_slave_flag() const { return (this->_ppuctrl() & 0b0100'0000) > 0; }
	inline bool sprite_height_flag() const { return (this->_ppuctrl() & 0b0010'0000) > 0; }
	inline bool background_pattern_table_select_flag() const
	{
		return (this->_ppuctrl() & 0b0001'0000) > 0;
	}
	inline bool sprite_tile_select_flag() const { return (this->_ppuctrl() & 0b0000'1000) > 0; }
	inline bool increment_mode_flag() const { return (this->_ppuctrl() & 0b0000'0100) > 0; }
	inline uint8_t nametable_select() const { return this->_ppuctrl() & 0b0000'0011; };

	//--------------------------------------------------------------------------
	// PPUSTATUS
	//--------------------------------------------------------------------------
	inline void vertical_blank_flag(const bool value)
	{
		if (value)
			this->_ppustatus(this->_ppustatus() | 0b1000'0000);
		else
			this->_ppustatus(this->_ppustatus() & 0b0111'1111);
	};
	inline void sprite_zero_hit_flag(const bool value)
	{
		if (value)
			this->_ppustatus(this->_ppustatus() | 0b0100'0000);
		else
			this->_ppustatus(this->_ppustatus() & 0b1011'1111);
	};
	inline void sprite_overflow_flag(const bool value)
	{
		if (value)
			this->_ppustatus(this->_ppustatus() | 0b0010'0000);
		else
			this->_ppustatus(this->_ppustatus() & 0b1101'1111);
	};

	//--------------------------------------------------------------------------
	// PPUMASK
	//--------------------------------------------------------------------------
	inline bool emphasize_blue_flag() const { return (this->_ppumask() & 0b1000'0000) > 0; };
	inline bool emphasize_green_flag() const { return (this->_ppumask() & 0b1000'0000) > 0; };
	inline bool emphasize_red_flag() const { return (this->_ppumask() & 0b0010'0000) > 0; };
	inline bool show_sprites_flag() const { return (this->_ppumask() & 0b0001'0000) > 0; };
	inline bool show_background_flag() const { return (this->_ppumask() & 0b0000'1000) > 0; };
	inline bool show_leftmost_sprites_flag() const { return (this->_ppumask() & 0b0000'0100) > 0; };
	inline bool show_leftmost_background_flag() const
	{
		return (this->_ppumask() & 0b0000'0010) > 0;
	};
	inline bool greyscale_flag() const { return (this->_ppumask() & 0b0000'0001) > 0; };

	//--------------------------------------------------------------------------
	void load_next_operation();
	void read_memory();

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

	void update_vout_registers();
};
