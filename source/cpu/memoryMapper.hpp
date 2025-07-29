#pragma once

#include <memory>

#include <base/iMemory.hpp>
#include <input/controllerPort.hpp>

class Cartridge;
class IDma;
class IInput;
class IPpu;

class MemoryMapper: public IMemory
{
public:
	MemoryMapper(IPpu* ppu, IDma* dma);
	virtual ~MemoryMapper();

	uint8_t read(uint16_t address) const override;
	void write(uint16_t address, const uint8_t data) override;

	void load_cartridge(Cartridge* cartridge);
	void unload_cartridge();

	void connect_controller(ControllerPort port, IInput* controller);
	void disconnect_controller(ControllerPort port);

private:
	std::unique_ptr<IMemory> _ram {nullptr};
	Cartridge* _cartridge {nullptr};
	IPpu* _ppu {nullptr};
	IDma* _dma {nullptr};
	IInput* _controller_1 {nullptr};
	IInput* _controller_2 {nullptr};

	uint8_t _data_latch {0x00};
};
