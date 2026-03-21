#pragma once

#include <vector>

/// <summary>
/// Basic info about video frame. Designed around moving the data to avoid accidental copies.
/// </summary>
struct Frame {
public:
	/// <summary>
	/// Each byte of MJPEG.
	/// </summary>
	std::vector<std::uint8_t> data;
	/// <summary>
	/// Timestamp of the frame.
	/// </summary>
	std::uint64_t timestamp;

	Frame() = default;
	Frame(std::vector<std::uint8_t>&& data, std::uint64_t timestamp) : data(std::move(data)), timestamp(timestamp) {};
	Frame(Frame&& frame) noexcept = default;
	Frame(const Frame& frame) = delete;

	Frame& operator=(Frame&& frame) noexcept = default;
	Frame& operator=(const Frame& frame) = delete;
};