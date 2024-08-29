#include "cpuRecorder.hpp"

#include <chrono>
#include <format>

namespace chrono = std::chrono;

namespace
{
struct StatusFlags
{
	bool n;
	bool v;
	bool b;
	bool d;
	bool i;
	bool z;
	bool c;
};

uint64_t getTime()
{
	auto now = chrono::system_clock::now();
	auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);

	auto value = now_ms.time_since_epoch();
	return static_cast<uint64_t>(value.count());
}

std::string getTimestamp()
{
	auto now = chrono::current_zone()->to_local(std::chrono::system_clock::now());
	return std::format("{:%F %T}", now);
}

std::string generateFilename()
{
	auto now = chrono::current_zone()->to_local(std::chrono::system_clock::now());
	return std::format("{:%Y%m%d_%H%M%S.log}", now);
}

StatusFlags parseStatusFlags(uint8_t state)
{
	return {
		(state & 0b10000000) > 0,
		(state & 0b01000000) > 0,
		(state & 0b00010000) > 0,
		(state & 0b00001000) > 0,
		(state & 0b00000100) > 0,
		(state & 0b00000010) > 0,
		(state & 0b00000001) > 0,
	};
}
}

CpuRecorder::CpuRecorder()
{
	this->_log_filename = generateFilename();
	this->openLogFile();
}

CpuRecorder::CpuRecorder(const std::string& log_filename): _log_filename(log_filename)
{
	this->openLogFile();
}

CpuRecorder::~CpuRecorder()
{
	this->_log_file.close();
}

void CpuRecorder::logFrame(const CpuStackFrame& frame)
{
	std::string row_1_data = std::format("{:#04x}  {:#04x}  {:#04x}  {:#04x}  {:#06x}", frame.a,
		frame.x, frame.y, frame.s, frame.program_counter);

	auto status = parseStatusFlags(frame.p);
	std::string row_2_data =
		std::format("{:#06x}  {:#04x}    {:b} {:b} x {:b} {:b} {:b} {:b} {:b}", frame.address_bus,
			frame.data_bus, status.n, status.v, status.b, status.d, status.i, status.z, status.c);

	this->_log_file << getTimestamp() << "\t\t" << row_1_data << "\t" << row_2_data << "\t"
					<< frame.instruction << std::endl;
	this->_log_file.flush();
}

void CpuRecorder::openLogFile()
{
	this->_log_file.open(this->_log_filename, std::ios::out | std::ios::trunc);
}
