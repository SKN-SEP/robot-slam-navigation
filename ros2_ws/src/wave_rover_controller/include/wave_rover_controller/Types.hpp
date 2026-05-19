#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

namespace WVController {
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
                << "}";
                
            out.cmd = ss.str();

            return out;
        };
    };

    enum class State {
        DISABLED,
        TELEOP
    };
}