#ifndef ROBOT_LOGIC_JOYSTICK_HANDLER_HPP
#define ROBOT_LOGIC_JOYSTICK_HANDLER_HPP

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"
#include "geometry_msgs/msg/twist.hpp"

class JoystickHandler : public rclcpp::Node {
public:
    JoystickHandler();

private:
    void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg) const;

    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
};

#endif