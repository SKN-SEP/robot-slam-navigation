#include "wave_rover_controller/SerialDriver.hpp"

WRController::SerialDriver::SerialDriver(const std::string& port, speed_t baudrate) : serialFd(-1), baudrate(baudrate), port(port) {};

WRController::SerialDriver::~SerialDriver() {
    if (this->serialFd >= 0) {
        close(this->serialFd);
    }
}

bool WRController::SerialDriver::Connect() {
    this->serialFd = open(
        this->port.c_str(),
        O_RDWR | O_NOCTTY | O_SYNC
    );

    if (serialFd < 0) return false;

    struct termios tty;

    if (tcgetattr(this->serialFd, &tty) != 0) return false;

    cfsetispeed(&tty, this->baudrate);
    cfsetospeed(&tty, this->baudrate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;

    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;

    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 5;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    return tcsetattr(this->serialFd, TCSANOW, &tty) == 0;
}

void WRController::SerialDriver::SendCommand(const CmdSpeedCtrl &cmd) const {
    if (this->serialFd < 0) return;

    write(
        this->serialFd,
        cmd.cmd.c_str(),
        cmd.cmd.size()
    );
}
