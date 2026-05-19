#pragma once

#include <string>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "ConnectionDriver.hpp"
#include "Types.hpp"

namespace WVController {
    class SerialDriver : public ConnectionDriver {
        private:
            int serialFd;
            speed_t baudrate;
            std::string port;
        public:
            SerialDriver(const std::string& port, speed_t baudrate);
            ~SerialDriver();

            bool Connect() override;
            void SendCommand(const CmdSpeedCtrl &cmd) const override;         
    };
}