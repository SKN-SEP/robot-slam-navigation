#pragma once

#include <memory>
#include <rclcpp/node.hpp>
#include <rclcpp/timer.hpp>
#include <rclcpp/time.hpp>
#include <termios.h>
#include "sensor_msgs/msg/joy.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "Types.hpp"
#include "wave_rover_controller/ConnectionDriver.hpp"
#include "wave_rover_controller/InputInterpreter.hpp"
#include "wave_rover_controller/MotionFactory.hpp"
#include "wave_rover_controller/StateMachine.hpp"
#include "wave_rover_controller/SerialDriver.hpp"
#include "wave_rover_controller/JoystickInputInterpreter.hpp"
#include "wave_rover_controller/InternalApi.hpp"

namespace WRController {
    class WaveRoverControllerNode : public rclcpp::Node {
        private:
            rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joySub;
            rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmdPub;
            rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imuPub;
            rclcpp::TimerBase::SharedPtr timer;
            rclcpp::TimerBase::SharedPtr imuTimer;

            std::unique_ptr<InputInterpreter> interpreter;
            std::unique_ptr<MotionFactory> motion;
            std::unique_ptr<StateMachine> stateMachine;
            std::unique_ptr<ConnectionDriver> driver;

            RawInput lastInput;

            void JoyCb(const sensor_msgs::msg::Joy::SharedPtr msg);
            void PublishCmd(const Motion & cmd);
            void PublishCmd(const ImuData & imu);
            void Loop();
            void ImuLoop();
        public:
            WaveRoverControllerNode();     
    };
}