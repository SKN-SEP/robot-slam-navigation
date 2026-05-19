#pragma once

#include "Types.hpp"

namespace WVController {
    class InputInterpreter {
        public:
            virtual ~InputInterpreter() = default;
            virtual Input Interpret(const RawInput &input) const = 0;
    };
}