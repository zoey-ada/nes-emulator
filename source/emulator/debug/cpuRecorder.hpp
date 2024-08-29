#pragma once

#include <fstream>
#include <string>

#include "cpuStackFrame.hpp"

class CpuRecorder
{
public:
	CpuRecorder();
	CpuRecorder(const std::string& log_filename);
	virtual ~CpuRecorder();

	void logFrame(const CpuStackFrame& frame);

private:
	std::string _log_filename;
	std::fstream _log_file;

	void openLogFile();
};
