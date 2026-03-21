#include "Input/FrameSources/DebugFrameSource.hpp"
#include "Containers/FrameSnapshotCyclicBuffer.hpp"
#include "Tasks/CameraDataRunner.hpp"
#include "Streaming/MjpegStreamingServer.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>

int main() {
    DebugFrameSource source;
    FrameSnapshotCyclicBuffer<Frame> buffer(150);
    CameraDataRunner runner(source, buffer);
    MjpegStreamingServer<Frame> server(buffer, "camera_frame", std::string(Config::ENDPOINT), Config::PORT);
    
    try {
        std::cout << "MjpegStreamingServer started.\n";

        runner.Start();
        server.Start(); // blocks

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
    }
}
