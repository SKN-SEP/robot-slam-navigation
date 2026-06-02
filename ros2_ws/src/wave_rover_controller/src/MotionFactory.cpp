#include "wave_rover_controller/MotionFactory.hpp"

WRController::MotionFactory::MotionFactory(const float ls, float as) : linearScale(ls), angularScale(as) {};

WRController::Motion WRController::MotionFactory::Create(const Input& input) const {
    Motion m;

    m.vx = input.x * this->linearScale;
    m.wz = input.z * this->angularScale;

    return m;
}