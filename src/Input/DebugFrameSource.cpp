#pragma warning(push)
#pragma warning(disable : 4996)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#pragma warning(pop)
#include "Input/FrameSources/DebugFrameSource.hpp"

DebugFrameSource::DebugFrameSource(std::size_t payloadSize) : payloadSize(payloadSize) {}

bool DebugFrameSource::Read(Frame& frame) {
	auto start = std::chrono::steady_clock::now();
	static std::mt19937_64 gen(std::random_device{}());

	std::vector<uint8_t> raw(Config::FRAME_SIZE);

	uint64_t* raw64 = reinterpret_cast<uint64_t*>(raw.data());
	size_t size64 = raw.size() / 8;

	for (size_t i = 0; i < size64; i++) raw64[i] = gen();

	size_t tail = raw.size() % 8;
	for (size_t i = raw.size() - tail; i < raw.size(); i++)
		raw[i] = static_cast<uint8_t>(gen());

	std::vector<uint8_t> data;

	stbi_write_jpg_to_func(
		[](void* context, void* data, int size) {
			auto* buffer = static_cast<std::vector<uint8_t>*>(context);
			uint8_t* bytes = static_cast<uint8_t*>(data);
			buffer->insert(buffer->end(), bytes, bytes + size);
		},
		&data,
		Config::FRAME_WIDTH,
		Config::FRAME_HEIGHT,
		Config::RGB_CHANNELS,
		raw.data(),
		10
	);

	auto now = std::chrono::steady_clock::now().time_since_epoch();
	auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(now).count();

	frame = Frame(std::move(data), static_cast<std::uint64_t>(timestamp));

	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
	std::cout << "[FPS]: " << std::chrono::milliseconds(1000) / elapsed << std::endl;

	return true;
};
