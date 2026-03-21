#include "Tasks/CameraDataRunner.hpp"

CameraDataRunner::CameraDataRunner(FrameSource& frameSource, FrameSnapshotCyclicBuffer<Frame>& buffer) : frameSource(frameSource), buffer(buffer) {}

void CameraDataRunner::Start() {
	if (this->running.exchange(true)) return;

	this->producer = std::thread(&CameraDataRunner::Run, this);
}

void CameraDataRunner::Stop() {
	if (!this->running.exchange(false)) return;

	if (this->producer.joinable()) this->producer.join();
}

void CameraDataRunner::Run() {
	Frame frame;

	while (this->running.load(std::memory_order_relaxed)) {
		if (this->frameSource.Read(frame)) buffer.Push(std::move(frame));
	}
};

CameraDataRunner::~CameraDataRunner() {
	this->Stop();
};