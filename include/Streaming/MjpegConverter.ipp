#pragma once

#include <ostream>

inline void MjpegConverter::ConvertFrame(const Frame& frame, std::ostream& out) const {
	//CLRF
	out << "--" << this->boundary << "\r\n";
	out << "Content-Type: image/jpeg\r\n";
	out << "Content-Length: " << frame.data.size() << "\r\n\r\n";

	out.write(reinterpret_cast<const char*>(frame.data.data()), static_cast<std::streamsize>(frame.data.size()));
	out << "\r\n";
	out.flush();
};
