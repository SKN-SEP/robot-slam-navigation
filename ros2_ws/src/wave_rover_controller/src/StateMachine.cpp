#include "wave_rover_controller/StateMachine.hpp"

WVController::StateMachine::StateMachine(const double timeout) : state(WVController::State::DISABLED), timeout(timeout) {
    this->lastUpdate = rclcpp::Time(0, 0, RCL_ROS_TIME);
};

void WVController::StateMachine::Update(bool active, bool valid, const rclcpp::Time &now) {
    if (!active || !valid) {
        this->state = State::DISABLED;
        return;
    }

    this->lastUpdate = now;
    this->state = State::TELEOP;
};

WVController::State WVController::StateMachine::GetState(const rclcpp::Time &now) const {
    if ((now - this->lastUpdate).seconds() > this->timeout) return State::DISABLED;
    
    return this->state;
};