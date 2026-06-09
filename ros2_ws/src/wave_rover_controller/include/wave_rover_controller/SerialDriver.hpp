#pragma once

#include <string>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "ConnectionDriver.hpp"
#include "Types.hpp"

namespace WRController {
    class SerialDriver : public ConnectionDriver {
        private:
            int serialFd;
            speed_t baudrate;
            std::string port;
        public:
            SerialDriver(const std::string& port, speed_t baudrate);
            ~SerialDriver();

            bool Connect() override;
            void SendCommand(const Cmd &cmd) const override;  
            std::string ReadResponse() const override;
    };
}