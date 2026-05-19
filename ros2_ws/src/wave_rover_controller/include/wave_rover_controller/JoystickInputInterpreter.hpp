#pragma once

#include "InputInterpreter.hpp"
#include "Types.hpp"

namespace WRController {
    class JoystickInputInterpreter : public InputInterpreter {
        public:
            Input Interpret(const RawInput &input) const override;
    };
}