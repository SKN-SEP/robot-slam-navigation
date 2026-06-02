#pragma once

#include "Types.hpp"

namespace WRController {
    class InputInterpreter {
        public:
            virtual ~InputInterpreter() = default;
            virtual Input Interpret(const RawInput &input) const = 0;
    };
}