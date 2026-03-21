#pragma once
#include <thread>
#include <atomic>
#include <iostream>
#include <string>
#include "httplib.h"
#include "Containers/FrameSnapshotCyclicBuffer.hpp"
#include "Streaming/MjpegConverter.hpp"
#include "Config.hpp"

template <typename T>
class MjpegStreamingServer {
private:
	FrameSnapshotCyclicBuffer<T>& buffer;
	MjpegConverter converter;
	std::thread server;
	std::atomic<bool> running{ false };
	std::string host;
	unsigned int port;

	void Run();
public:
	MjpegStreamingServer(FrameSnapshotCyclicBuffer<T>& buffer, std::string boundary = "camera_frame", std::string host = "0.0.0.0", int port = 8000);
	
	void Start();
	void Stop();
	
	~MjpegStreamingServer();
};

#include "Streaming/MjpegStreamingServer.ipp"