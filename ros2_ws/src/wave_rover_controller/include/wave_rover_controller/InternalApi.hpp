#pragma once

#include <string>

namespace WRController {
    class InternalApi {
        public:
            static double Extract(const std::string& haystack, const std::string& needle) {
                auto pos = haystack.find(needle);

                if (pos == std::string::npos) return 0;

                pos = haystack.find(':', pos);
                
                if (pos == std::string::npos) return 0;

                size_t end = haystack.find_first_of(",", pos);

                return std::stod(haystack.substr(pos + 1, end - pos - 1));
            }
    };
}