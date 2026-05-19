#include "wave_rover_controller/JoystickInputInterpreter.hpp"

WVController::Input WVController::JoystickInputInterpreter::Interpret(const RawInput &input) const {
    WVController::Input out;

    //invalid message
    if (input.axes.size() < 2 || input.buttons.size() < 6) {
        return out;
    }

    out.x = input.axes[1];
    out.z = input.axes[0];
    out.controllerPressed = (input.buttons[5] == 1);
    out.valid = true;

    return out;
}