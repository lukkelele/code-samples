#pragma once

#include "core/core.h"

template<typename T, std::size_t N>
class ring_buffer
{
public:
	static_assert(N > 0, "Capacity must be greater than 0");

public:
	ring_buffer() = default;
	~ring_buffer() = default;

	std::size_t size() const { return count; }
	constexpr std::size_t capacity() const { return N; }
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

		pending[tail] = item;
		if constexpr (std::is_integral_v<T>) {
			std::println("[ring_buffer::put] item={} head={} tail={} count={}", item, head, tail, count); /* @todo: REMOVE */
		}
		tail = next_index(tail);
		count++;
		return true;
	}

	bool get(T& out_item)
	{
		if (empty()) {
			return false;
		}

		out_item = pending[head];
		if constexpr (std::is_integral_v<T>) {
			std::println("[ring_buffer::get] item={} head={} tail={} count={}", out_item, head, tail, count); /* @todo: REMOVE */
		}
		head = next_index(head);
		count--;
		return true;
	}

	T* front()
	{
		if (empty()) {
			return NULL;
		}

		return &pending[head];
	}

	const T* front() const
	{
		if (empty()) {
			return NULL;
		}

		return &pending[head];
	}

	std::array<T, N>::iterator begin() { return pending.begin(); }
	std::array<T, N>::iterator end() { return pending.end(); }
	std::array<T, N>::const_iterator begin() const { return pending.begin(); }
	std::array<T, N>::const_iterator end() const { return pending.end(); }
	std::array<T, N>::const_iterator cbegin() const { return pending.cbegin(); }
	std::array<T, N>::const_iterator cend() const { return pending.cend(); }

private:
	/**
	 * @brief Get next index, wraps around at N.
	 */
	std::size_t next_index(std::size_t index) const
	{
		index++;
		if (index == N) {
			index = 0;
		}
		return index;
	}

private:
	std::array<T, N> pending{};
	std::size_t count = 0; /* 0 <= count <= N */
	std::size_t head = 0;
	std::size_t tail = 0;
};
