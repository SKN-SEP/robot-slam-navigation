#pragma once

#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

namespace WRController {
    struct Input {
        float x = 0.0f;
        float z = 0.0f;
        bool controllerPressed = false;
        bool valid = false;
    };

    struct RawInput {
        std::vector<float> axes;
        std::vector<int> buttons;
    };

    struct Cmd {
        std::string cmd; 
    };

    struct CmdSpeedCtrl : public Cmd {
        float lWheel = 0.0f;
        float rWheel = 0.0f;
    };

    struct CmdIMUReceive : public Cmd {
        CmdIMUReceive() {
            this->cmd = "{\"T\":126}\r\n";
        };
    };

    struct Motion {
        float vx = 0.0f;
        float wz = 0.0f;

        operator CmdSpeedCtrl() const {
            CmdSpeedCtrl out;

            out.lWheel = std::clamp(this->vx - this->wz, -0.5f, 0.5f);
            out.rWheel = std::clamp(this->vx + this->wz, -0.5f, 0.5f);

            std::stringstream ss;
            
            ss 
                << "{"
                << "\"T\":1,"
                << "\"L\":" << out.lWheel << ","
                << "\"R\":" << out.rWheel
                << "}\r\n";
                
            out.cmd = ss.str();

            return out;
        };
    };

    struct ImuData {
        double roll = 0.0;
        double pitch = 0.0;
        double yaw = 0.0;
        double ax = 0.0;
        double ay = 0.0;
        double az = 0.0;
        double gx = 0.0;
        double gy = 0.0;
        double gz = 0.0;
    };

    enum class State {
        DISABLED,
        TELEOP
    };

    static constexpr double DEG_TO_RAD = M_PI / 180;
    static constexpr double M_S2_FACTOR = 0.01;
}