#include "core/core.h"
#include "ring_buffer.h"

/** @todo: Format logging */
template<typename T, std::size_t N>
static void print_ring_buffer(const ring_buffer<T, N>& buf)
{
	int idx = 0;
	if constexpr (std::is_integral_v<T>) {
		for (const T& entry : buf) {
			std::printf("buf[%d]: %d\n", idx++, entry);
		}
	} else {
		static_assert(false, "Not supported");
	}
}

static void test_ring_buffer();

int main(int argc, char** argv)
{
	core::print_banner("experimental: event_system");

	test_ring_buffer();

	return 0;
}


static void test_ring_buffer()
{
	core::log("create ring_buffer<int>");
	ring_buffer<int, 4> buf;
	buf.put(1);
	buf.put(4);
	buf.put(9);

	print_ring_buffer(buf);

	std::printf("buf.front() == %d\n", *buf.front());

	int x = 0;
	if (buf.get(x)) {
		print_ring_buffer(buf);
		std::printf("\n");
	}

	if (buf.get(x)) {
		print_ring_buffer(buf);
		std::printf("\n");
	}

	if (buf.get(x)) {
		print_ring_buffer(buf);
		std::printf("\n");
	}

	if (buf.put(32)) {
		print_ring_buffer(buf);
		std::printf("\n");
	}

	if (buf.put(65)) {
		print_ring_buffer(buf);
		std::printf("\n");
	}
}
