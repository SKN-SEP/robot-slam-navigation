#pragma once

#include <atomic>
#include <memory>

/// <summary>
/// Memory cell of the buffer to avoid multiple re-allocations upon overwriting. Uses sequence numbers to control cell ordering.
/// </summary>
/// <typeparam name="T">Type of the memory cell (will follow CyclicBuffer type)</typeparam>
template<typename T>
struct CyclicBufferCell {
public:
	/// <summary>
	/// Value of the memory cell.
	/// </summary>
	std::unique_ptr<T> value;
	/// <summary>
	/// Current sequence number of the memory cell -> https://algassert.com/math/2014/03/11/Ordering-Cyclic-Sequence-Numbers.html.
	/// </summary>
	std::atomic<std::uint64_t> sequenceNumber;
};