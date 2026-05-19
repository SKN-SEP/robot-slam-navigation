#pragma once

#include "Types.hpp"

namespace WRController {
    class MotionFactory {
        private:
            const float linearScale;
            const float angularScale; 
        public:
            MotionFactory(const float ls, float as);
            Motion Create(const Input& input) const;        
    };
}