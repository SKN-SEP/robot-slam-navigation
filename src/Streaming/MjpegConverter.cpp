#include "Streaming/MjpegConverter.hpp"

MjpegConverter::MjpegConverter(std::string boundary) : boundary(std::move(boundary)) {}

const std::string& MjpegConverter::GetBoundary() const noexcept {
	return this->boundary;
};