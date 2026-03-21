# robot-slam-navigation-webserver-module

## Installation

Download the project and compile it with any compiler (C++17>=). Module uses **HOL** (Header-only Libraries), therefore no additional including nor CMake is required.

## Usage

For now, configuration is available in `include/Config.hpp` where endpoint, port and frame size can be adjusted. 

It's simple to turn on the module - just run the file and the live camera stream (for now debug view) is sent to the endpoint set in config.

With default config the endpoint is located at `http://localhost:8000/camera` and the stream can be viewed by just opening `webapp/index.html` page. Give it a try.