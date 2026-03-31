cmake_minimum_required(VERSION 3.8)
project(robot_logic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

# Add the executable
add_executable(joystick_handler src/joystick_handler.cpp)

# Link dependencies
ament_target_dependencies(joystick_handler rclcpp sensor_msgs geometry_msgs)

# Tell CMake where to find your headers
target_include_directories(joystick_handler PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Install the executable
install(TARGETS joystick_handler
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()