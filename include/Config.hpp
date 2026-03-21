#pragma once

#include <cstdint>
#include <string_view>

/// <summary>
/// Configuration of the WebServer.
/// </summary>
struct Config {
public:
	Config() = delete;
	/// <summary>
	/// Compiled-time known frame width.
	/// </summary>
	static constexpr std::uint16_t FRAME_WIDTH = 640;
	/// <summary>
	/// Compiled-time known frame height.
	/// </summary>
	static constexpr std::uint16_t FRAME_HEIGHT = 480;
	/// <summary>
	/// Compiled-time known frame RGB channels.
	/// </summary>
	static constexpr std::uint8_t RGB_CHANNELS = 3;
	/// <summary>
	/// Compiled-time known size of the frame.
	/// </summary>
	static constexpr std::uint64_t FRAME_SIZE = Config::FRAME_WIDTH * Config::FRAME_HEIGHT * Config::RGB_CHANNELS;
	static constexpr std::string_view ENDPOINT = "0.0.0.0";
	static constexpr unsigned int PORT = 8000;
	static constexpr std::string_view STREAM_ROUTE = "/camera";

};