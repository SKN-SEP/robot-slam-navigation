#include "robot_logic/joystick_handler.hpp"

JoystickHandler::JoystickHandler() : Node("joystick_handler") {
    // Parameters
    this->declare_parameter("linear_speed", 0.75);
    this->declare_parameter("angular_speed", 1.5);

    // Publisher and subscriber
    sub_ = this->create_subscription<sensor_msgs::msg::Joy>(
        "joy", 10, std::bind(&JoystickHandler::joy_callback, this, std::placeholders::_1));
    
    pub_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    RCLCPP_INFO(this->get_logger(), "Joystick handler node has been initialized!");
}

void JoystickHandler::joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg) const {
    auto twist = geometry_msgs::msg::Twist();
    
    double l_scale = this->get_parameter("linear_speed").as_double();
    double a_scale = this->get_parameter("angular_speed").as_double();

    twist.linear.x = msg->axes[1] * l_scale;
    twist.angular.z = msg->axes[0] * a_scale;

    // Deadman switch (Button 5 = R1/RB)
    if (msg->buttons[5] == 1)
        pub_->publish(twist);
    else 
        pub_->publish(geometry_msgs::msg::Twist());
}

// Main function lives here at the bottom
int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JoystickHandler>());
    rclcpp::shutdown();
    return 0;
}