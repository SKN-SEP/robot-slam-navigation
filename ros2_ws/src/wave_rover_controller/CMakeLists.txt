cmake_minimum_required(VERSION 3.8)
project(wave_rover_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# ===== EXECUTABLE =====
add_executable(wave_rover_controller_node
  src/WaveRoverControllerNode.cpp
  src/JoystickInputInterpreter.cpp
  src/MotionFactory.cpp
  src/StateMachine.cpp
  src/SerialDriver.cpp
)

ament_target_dependencies(wave_rover_controller_node
  rclcpp
  sensor_msgs
  geometry_msgs
  std_msgs
)

# include directory (important for headers)
target_include_directories(wave_rover_controller_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# install
install(TARGETS
  wave_rover_controller_node
  DESTINATION lib/${PROJECT_NAME}
)

# optional: headers
install(DIRECTORY include/
  DESTINATION include
)

ament_package()