#pragma once

#include <memory>
#include <rclcpp/node.hpp>
#include <rclcpp/timer.hpp>
#include <rclcpp/time.hpp>
#include <termios.h>
#include "sensor_msgs/msg/joy.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "Types.hpp"
#include "wave_rover_controller/ConnectionDriver.hpp"
#include "wave_rover_controller/InputInterpreter.hpp"
#include "wave_rover_controller/MotionFactory.hpp"
#include "wave_rover_controller/StateMachine.hpp"
#include "wave_rover_controller/SerialDriver.hpp"
#include "wave_rover_controller/JoystickInputInterpreter.hpp"

namespace WVController {
    class WaveRoverControllerNode : public rclcpp::Node {
        private:
            rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joySub;
            rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmdPub;
            rclcpp::TimerBase::SharedPtr timer;

            std::unique_ptr<InputInterpreter> interpreter;
            std::unique_ptr<MotionFactory> motion;
            std::unique_ptr<StateMachine> stateMachine;
            std::unique_ptr<ConnectionDriver> driver;

            RawInput lastInput;

            void JoyCb(const sensor_msgs::msg::Joy::SharedPtr msg);
            void PublishCmd(const Motion & cmd);
            void Loop();
        public:
            WaveRoverControllerNode();     
    };
}