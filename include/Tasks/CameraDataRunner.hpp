#pragma once

#include "Input/FrameSources/FrameSource.hpp"
#include "Containers/FrameSnapshotCyclicBuffer.hpp"
#include "Entities/Frame.hpp"
#include <atomic>
#include <thread>

class CameraDataRunner {
private:
	FrameSource& frameSource;
	FrameSnapshotCyclicBuffer<Frame>& buffer;
	std::atomic<bool> running{ false };
	std::thread producer;

	void Run();
public:
	CameraDataRunner(FrameSource& frameSource, FrameSnapshotCyclicBuffer<Frame>& buffer);
	CameraDataRunner(const CameraDataRunner& runner) = delete;

	CameraDataRunner& operator=(const CameraDataRunner&) = delete;

	void Start();
	void Stop();

	~CameraDataRunner();
};