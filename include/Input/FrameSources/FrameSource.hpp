#pragma once

#include "Entities/Frame.hpp"

/// <summary>
/// Abstract class designing the source of the frames (eg. camera or debug data).
/// </summary>
class FrameSource {
public:
	virtual ~FrameSource() = default;
	/// <summary>
	/// Read from the input device current image frame.
	/// </summary>
	/// <param name="frame">Output frame.</param>
	/// <returns>Whether the read was successful.</returns>
	virtual bool Read(Frame& frame) = 0;
};