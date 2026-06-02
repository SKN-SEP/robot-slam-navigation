#pragma once

#include "Types.hpp"

namespace WRController {
    class ConnectionDriver {
        public:
            virtual ~ConnectionDriver() = default;
            virtual bool Connect() = 0;
            virtual void SendCommand(const CmdSpeedCtrl &cnd) const = 0;
    };
}