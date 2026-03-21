#pragma once

#include <string>
#include <fstream>
#include "Containers/FrameSnapshotCyclicBuffer.hpp"
#include "Entities/Frame.hpp"

class MjpegConverter {
private:
	std::string boundary;
public:
	explicit MjpegConverter(std::string boundary);

	void ConvertFrame(const Frame& frame, std::ostream& out) const;
	const std::string& GetBoundary() const noexcept;
};

#include "Streaming/MjpegConverter.ipp"