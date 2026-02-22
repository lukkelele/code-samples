#pragma once

#include <atomic>
#include <limits>

#include "core/core.h"

#define RING_BUF_DEBUG

template<typename T, std::size_t N, typename size_type = std::size_t>
	requires core::Formattable<T>
class ring_buffer
{
private:
	static_assert(N > 0, "Capacity must be greater than 0");
	static_assert(std::is_integral_v<size_type>);
	static_assert(std::is_unsigned_v<size_type>);
	static_assert(sizeof(size_type) <= sizeof(std::size_t), "size_type is too large, it has to respect the buffer constraints");

public:
	ring_buffer() = default;
	~ring_buffer() = default;

	size_type size() const { return count; }
	constexpr size_type capacity() const { return N; }
	bool full() const { return count == N; }
	bool empty() const { return count == 0; }

	void clear()
	{
		head = 0;
		tail = 0;
		count = 0;
	}

	bool put(const T& item)
	{
		if (full()) {
			return false;
		}

		data[tail] = item;
		tail = next_index(tail);
		count++;
#ifdef RING_BUF_DEBUG
		std::println("[ring_buffer::put] item={} head={} tail={} count={}", item, head, tail, count); /* @todo: REMOVE */
		debug_print();
#endif
		return true;
	}

	bool put_front(const T& item)
	{
		if (full()) {
			return false;
		}

		head = prev_index(head);
		data[head] = item;
		count++;
#ifdef RING_BUF_DEBUG
		std::println("[ring_buffer::put_front] item={} head={} tail={} count={}", item, head, tail, count); /* @todo: REMOVE */
		debug_print();
#endif
		return true;
	}

	bool get(T& out_item)
	{
		if (empty()) {
			return false;
		}

		out_item = std::move(data[head]);
		head = next_index(head);
		count--;
#ifdef RING_BUF_DEBUG
		std::println("[ring_buffer::get] item={} head={} tail={} count={}", out_item, head, tail, count); /* @todo: REMOVE */
		debug_print();
#endif
		return true;
	}

	T* front()
	{
		if (empty()) {
			return nullptr;
		}

		return &data[head];
	}

	const T* front() const
	{
		if (empty()) {
			return nullptr;
		}

		return &data[head];
	}

	/** @fixme: Does not iterate in FIFO order, should be able to iterate the logical sequence */
	std::array<T, N>::iterator begin() { return data.begin(); }
	std::array<T, N>::iterator end() { return data.end(); }
	std::array<T, N>::const_iterator begin() const { return data.begin(); }
	std::array<T, N>::const_iterator end() const { return data.end(); }
	std::array<T, N>::const_iterator cbegin() const { return data.cbegin(); }
	std::array<T, N>::const_iterator cend() const { return data.cend(); }

#ifdef RING_BUF_DEBUG
	void debug_print() const
	{
		std::size_t idx = 0;
		for (const T& entry : data) {
			if ((idx == head) && (idx == tail)) {
				std::println("ring_buf[{}]: {} <-- head & tail", idx++, entry);
			} else if (idx == head) {
				std::println("ring_buf[{}]: {} <-- head", idx++, entry);
			} else if (idx == tail) {
				std::println("ring_buf[{}]: {} <-- tail", idx++, entry);
			} else {
				std::println("ring_buf[{}]: {}", idx++, entry);
			}
		}
		std::println();
	}
#endif

private:
	/**
	 * @brief Get next index, wraps around at N.
	 */
	size_type next_index(size_type index) const
	{
		index++;
		if (index == N) {
			index = 0;
		}
		return index;
	}

	/**
	 * @brief Get previous index, wraps around at 0.
	 */
	size_type prev_index(size_type index) const
	{
		if (index == 0) {
			return N - 1;
		}
		return index - 1;
	}

private:
	std::array<T, N> data{};
	size_type count = 0; /* 0 <= count <= N */
	size_type head = 0;
	size_type tail = 0;
};
