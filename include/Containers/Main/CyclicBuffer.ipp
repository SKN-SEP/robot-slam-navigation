template<typename T>
inline CyclicBuffer<T>::CyclicBuffer(std::size_t capacity) : capacity(capacity), cells(std::make_unique<CyclicBufferCell<T>[]>(capacity)) {}

template<typename T>
inline void CyclicBuffer<T>::Push(T&& val) {
	auto sequence = this->sequenceNumber.fetch_add(1, std::memory_order_acq_rel);
	auto& cell = this->cells[sequence % this->capacity];

	cell.value = std::make_unique<T>(std::move(val));
	cell.sequenceNumber.store(sequence + 1, std::memory_order_release);
}

