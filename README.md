# Mobile Robot SLAM & Navigation
This repository contains a ROS 2 Jazzy package for a 4WD differential drive robot. The project contains simulation in Gazebo Harmonic.

## Getting Started
Before cloning this repository, you need to follow <a href="https://docs.ros.org/en/jazzy/Installation.html">installation guide for deb packages</a>.

### Prerequisites
- **OS**: Ubuntu 24.04 (Noble Numbat);
- **ROS2**: Jazzy Jalisco;
- **Simulator**: Gazebo Harmonic;
- **Bridge**: ros_gz_bridge;

## Gazebo Harmonic Simulation
The simulation environment is built using Gazebo Harmonic. It provides a physics-accurate 3D world where the 4WD robot can interact with objects and generate sensor data.

### Launching the Simulation
To start the robot and the simulation environment, run:
```
cd ./ros2_ws
source install/setup.bash
ros2 launch robot_sim gazebo_model.py
```
> On systems with NVIDIA GPUs, if the Gazebo window crashes on startup, Add this line to your ~/.bashrc file: <br>
> ***export __EGL_VENDOR_LIBRARY_FILENAMES=/usr/share/glvnd/egl_vendor.d/10_nvidia.json***

### Rebuilding the ROS2 package
Sometimes we need to update our code, in order to apply changes go to ros2_ws and run:
```
rm -rf build/ install/ log/
colcon build --symlink-install
source install/setup.bash
```

### Controlling the Robot
Once the simulation is running and you can drive the robot using a joystick node. Run the following command to install the necessary packages:
```
sudo apt update
sudo apt install ros-jazzy-joy ros-jazzy-joy-teleop ros-jazzy-teleop-tools
```
Run these, to control the robot:
```
ros2 run joy joy_node
```
```
ros2 run robot_logic joystick_handler
```

### Controlling the Robot via joystick
WaveRoverControllerNode allows to physically control the chasis movement with joystick while bypassing the need of having the simulation. It provides State Machine design allowing the movement only when safety button is pressed (RB) and joystick input data is valid.

To run the WaveControllerNode you need active joy_node:
```
ros2 run joy joy_node
```
And then run the WaveControllerNode:
```
ros2 run wave_rover_controller wave_rover_controller_node
```
The live processed input data of the joystick (not the values that are sent to CMD\_SPEED\_CTRL) is available at:
```
ros2 topic echo /wave_rover/cmd_vel
```

