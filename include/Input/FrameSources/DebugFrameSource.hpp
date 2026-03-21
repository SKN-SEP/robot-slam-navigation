#pragma once

#include "Input/FrameSources/FrameSource.hpp"
#include "Config.hpp"
#include <atomic>
#include <chrono>
#include <fstream>
#include <random>
#include <iostream>

/// <summary>
/// FrameSource for debugging purposes generating JPEG image sequences with garbage data. Useful for testing WebServer <-> WebEndpoint connection.
/// </summary>
class DebugFrameSource final : public FrameSource {
private:
	std::size_t payloadSize;
public:
	explicit DebugFrameSource(std::size_t payloadSize = 1024);

	bool Read(Frame& frame) override;
};