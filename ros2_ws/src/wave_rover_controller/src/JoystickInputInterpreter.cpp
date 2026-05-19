#include "wave_rover_controller/JoystickInputInterpreter.hpp"

WRController::Input WRController::JoystickInputInterpreter::Interpret(const RawInput &input) const {
    WRController::Input out;

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