#include "wave_rover_controller/StateMachine.hpp"

WRController::StateMachine::StateMachine(const double timeout) : state(WRController::State::DISABLED), timeout(timeout) {
    this->lastUpdate = rclcpp::Time(0, 0, RCL_ROS_TIME);
};

void WRController::StateMachine::Update(bool active, bool valid, const rclcpp::Time &now) {
    if (!active || !valid) {
        this->state = State::DISABLED;
        return;
    }

    this->lastUpdate = now;
    this->state = State::TELEOP;
};

WRController::State WRController::StateMachine::GetState(const rclcpp::Time &now) const {
    if ((now - this->lastUpdate).seconds() > this->timeout) return State::DISABLED;
    
    return this->state;
};