#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rclcpp/time.hpp>
#include "Types.hpp"

// TODO: Autonomus state and EmergencyStop state if needed.
namespace WVController {
    class StateMachine {
        private:
            State state;
            rclcpp::Time lastUpdate;
            const double timeout;
            bool hasUpdate = false;
        public:
            StateMachine(const double timeout);
            void Update(bool active, bool valid, const rclcpp::Time &now);
            State GetState(const rclcpp::Time &now) const;    
    };
}