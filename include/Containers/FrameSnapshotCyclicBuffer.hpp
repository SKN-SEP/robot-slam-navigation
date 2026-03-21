#pragma once

#include "Containers/Main/CyclicBuffer.hpp"

/// <summary>
/// 
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class FrameSnapshotCyclicBuffer final : public CyclicBuffer<T> {
public:
	/// <summary>
	/// 
	/// </summary>
	class FrameSnapshot {
	private:
		const FrameSnapshotCyclicBuffer* buffer;
		std::uint64_t _start;
		std::uint64_t _end;

		FrameSnapshot(const FrameSnapshotCyclicBuffer* buffer, std::uint64_t start, std::uint64_t end) : buffer(buffer), _start(start), _end(end) {};

		friend class FrameSnapshotCyclicBuffer;
	public:
		/// <summary>
		/// Iterator for FrameSnapshot. Uses C++17 tagging, might switch into C++20 concepts design eventually in the future.
		/// </summary>
		struct Iterator {
		private:
			const FrameSnapshotCyclicBuffer<T>* buffer;
			std::uint64_t sequenceNumber;
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = const T;
			using difference_type = std::ptrdiff_t;
			using pointer = const T*;
			using reference = const T&;

			Iterator(const FrameSnapshotCyclicBuffer* buffer, std::uint64_t sequenceNumber) : buffer(buffer), sequenceNumber(sequenceNumber) {};

			reference operator*() const { return *this->buffer->cells[this->sequenceNumber % this->buffer->capacity].value; };
			pointer operator->() const { return &(**this); }
			Iterator& operator++() { ++this->sequenceNumber; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
			bool operator==(const Iterator& other) const { return this->sequenceNumber == other.sequenceNumber; }
			bool operator!=(const Iterator& other) const { return this->sequenceNumber != other.sequenceNumber; }
		};

		Iterator begin() const noexcept { return Iterator(this->buffer, this->_start); }
		Iterator end() const noexcept { return Iterator(this->buffer, this->_end); }
	};

	explicit FrameSnapshotCyclicBuffer(std::size_t capacity) : CyclicBuffer<T>(capacity) {};
	FrameSnapshotCyclicBuffer(FrameSnapshotCyclicBuffer&& buffer) noexcept = default;
	FrameSnapshotCyclicBuffer(const FrameSnapshotCyclicBuffer & buffer) = delete;

	FrameSnapshotCyclicBuffer& operator=(FrameSnapshotCyclicBuffer && buffer) noexcept = default;
	FrameSnapshotCyclicBuffer& operator=(const FrameSnapshotCyclicBuffer & buffer) = delete;

	FrameSnapshot Snapshot() const noexcept {
		auto end = this->sequenceNumber.load(std::memory_order_acquire);
		auto start = (end > this->capacity) ? (end - this->capacity) : 0;

		return FrameSnapshot(this, start, end);
	}
};