#pragma once

#include "Types.hpp"

namespace WRController {
    class ConnectionDriver {
        public:
            virtual ~ConnectionDriver() = default;
            virtual bool Connect() = 0;
            virtual void SendCommand(const Cmd&cmd) const = 0;
            virtual std::string ReadResponse() const = 0;
    };
}