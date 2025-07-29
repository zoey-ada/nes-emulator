#pragma once

#include <array>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>

#include <base/iPpu.hpp>
#include <base/register.hpp>

#include "ppuCtrlRegister.hpp"
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
	explicit PictureProcessingUnit(IMemory* memory, IMemory* oam);
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
	void ppu_mask(const uint8_t value) { this->_ppumask(value); }
	uint8_t ppu_status();
	void oam_addr(const uint8_t value) { this->_oam_address(value); }
	uint8_t oam_addr() const override { return this->_oam_address(); }
	uint8_t oam_data() const;
	void oam_data(const uint8_t value);
	void ppu_scroll(const uint8_t value);
	void ppu_addr(const uint8_t value);
	uint8_t ppu_data();
	void ppu_data(const uint8_t value);

protected:
	IMemory* _memory {nullptr};
	IMemory* _object_attribute_memory {nullptr};                            // 64 sprites * 4 bytes
	std::unique_ptr<IMemory> _secondary_object_attribute_memory {nullptr};  // 8 sprite cache
	std::unique_ptr<SystemPalette> _sys_palette {nullptr};

	PpuCtrlRegister _ppuctrl;

	Register_14bit _address_bus;
	// TODO: technically this is supposed to be the lower byte of the address bus
	Register_8bit _data_bus;

	Register_3bit _cpu_address_bus;
	Register_8bit _cpu_data_bus;

	virtual void write_memory();

private:
	Register_8bit _ppumask;
	Register_8bit _ppustatus;
	Register_8bit _ppu_data_read_buffer;

	Register_15bit _vram_address;
	Register_15bit _temp_vram_address;
	Register_3bit _fine_x_scroll;
	bool _second_write_flag {false};

	Register _nametable_latch {0x00};
	Register _attribute_table_latch {0x00};
	bool _x_scroll_attribute_flag {false};
	bool _y_scroll_attribute_flag {false};
	Register _pattern_table_tile_low_latch {0x00};
	Register _pattern_table_tile_high_latch {0x00};

	bool _read_write {false};

	ICpu* _cpu {nullptr};

	// argb format (a is ignored)
	uint32_t _vout {0x00000000};
	DoubleChunkPixels _vout_shift_register;
	uint8_t _vout_shift_register_index = 0;
	ChunkIndices _background_index_latch;
	std::array<ChunkIndices, 8> _sprite_index_latches;

	std::deque<Action> _actions;
	std::deque<Action> _background_actions;
	std::deque<Action> _sprite_actions;

	// sprites
	Register_8bit _oam_address;
	Register_5bit _secondary_oam_address;
	bool _oam_force_ff_read {false};
	bool _secondary_oam_readonly {false};
	uint8_t _sprite_data_latch {0x00};

	std::array<uint8_t, 8> _sprite_attribute_latches;
	std::array<uint8_t, 8> _sprite_x_pos_latches;
	uint8_t _sprite_y_pos_latch;
	uint8_t _sprite_pt_index_latch;
	uint8_t _sprite_pt_high_latch;
	uint8_t _sprite_pt_low_latch;
	bool _sprite_0_hit {false};

	uint16_t _row {0};
	uint16_t _column {0};
	bool _is_even_frame {false};

	bool _is_starting_up {true};
	uint64_t _startup_delay {ppu_startup_delay};

	//--------------------------------------------------------------------------
	// PPUSTATUS
	//--------------------------------------------------------------------------
	inline void vertical_blank_flag(const bool value)
	{
		if (value)
			this->_ppustatus(this->_ppustatus() | 0b1000'0000);
		else
			this->_ppustatus(this->_ppustatus() & 0b0111'1111);
	}
	inline void sprite_zero_hit_flag(const bool value)
	{
		if (value)
			this->_ppustatus(this->_ppustatus() | 0b0100'0000);
		else
			this->_ppustatus(this->_ppustatus() & 0b1011'1111);
	}
	inline void sprite_overflow_flag(const bool value)
	{
		if (value)
			this->_ppustatus(this->_ppustatus() | 0b0010'0000);
		else
			this->_ppustatus(this->_ppustatus() & 0b1101'1111);
	}

	//--------------------------------------------------------------------------
	// PPUMASK
	//--------------------------------------------------------------------------
	inline bool emphasize_blue_flag() const { return (this->_ppumask() & 0b1000'0000) > 0; }
	inline bool emphasize_green_flag() const { return (this->_ppumask() & 0b1000'0000) > 0; }
	inline bool emphasize_red_flag() const { return (this->_ppumask() & 0b0010'0000) > 0; }
	inline bool show_sprites_flag() const { return (this->_ppumask() & 0b0001'0000) > 0; }
	inline bool show_background_flag() const { return (this->_ppumask() & 0b0000'1000) > 0; }
	inline bool show_leftmost_sprites_flag() const { return (this->_ppumask() & 0b0000'0100) > 0; }
	inline bool show_leftmost_background_flag() const
	{
		return (this->_ppumask() & 0b0000'0010) > 0;
	};
	inline bool greyscale_flag() const { return (this->_ppumask() & 0b0000'0001) > 0; }

	inline bool rendering_enabled() const { return (this->_ppumask() & 0b0001'1000) > 0; }

	//--------------------------------------------------------------------------
	void load_next_operation();
	void load_next_background_operation();
	void load_next_sprite_operation();

	void read_memory();
	uint8_t read_oam() const;
	uint8_t read_secondary_oam() const;
	void write_secondary_oam(const uint8_t data);

	void feed_vout();

	void nametable_read();
	void attribute_table_read();
	void process_visible_pixels();
	void render_chunk();

	void clear_secondary_oam();
	void evaluate_sprites();
	void copy_sprite_data();
	void load_sprite();
	inline uint8_t get_sprite_index() const
	{
		return (this->_secondary_oam_address() & 0b0001'1100) >> 2;
	}
	inline uint8_t get_prev_sprite_index() const
	{
		auto sprite_index = this->get_sprite_index();
		return sprite_index == 0 ? 7 : sprite_index - 1;
	}

	ChunkIndices compile_pattern_table_bytes() const;
	ChunkIndices compile_sprite_pt_bytes() const;
	uint8_t parse_background_palette_number() const;
	uint8_t parse_sprite_palette_number() const;
	uint32_t apply_palette_color(const uint8_t& index, const uint8_t palette_number) const;
	ChunkPixels apply_palette_colors(const ChunkIndices& indices,
		const uint8_t palette_number) const;
	void fetch_sprite_data();

	uint16_t calculate_pattern_table_address(bool bit_plane);
	uint16_t calclutate_sprite_pt_address(bool bit_plane);
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

	ChunkPixels determine_output_pixels();
	void update_vout_registers();
};
