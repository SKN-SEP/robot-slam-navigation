#pragma once

#include "CyclicBufferCell.hpp"
#include <cstddef>
#include <atomic>
#include <memory>

template<typename T>
class CyclicBuffer {
protected:
	const std::size_t capacity;
	std::unique_ptr<CyclicBufferCell<T>[]> cells;
	std::atomic<std::uint64_t> sequenceNumber{0};
public:
	explicit CyclicBuffer(std::size_t capacity);
	CyclicBuffer(CyclicBuffer&& buffer) noexcept = default;
	CyclicBuffer(const CyclicBuffer& buffer) = delete;
	
	CyclicBuffer& operator=(CyclicBuffer&& buffer) noexcept = default;
	CyclicBuffer& operator=(const CyclicBuffer& buffer) = delete;

	void Push(T&& val);

	~CyclicBuffer() = default;
};

#include "CyclicBuffer.ipp"
