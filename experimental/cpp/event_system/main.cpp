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

int main(int argc, char** argv)
{
	core::print_banner("experimental: event_system");

	{
		core::log("create ring_buffer<int>");
		ring_buffer<int, 4> buf;
		buf.push_back(1);
		buf.push_back(4);
		buf.push_back(9);

		print_ring_buffer(buf);

		std::printf("buf.front() == %d\n", *buf.front());
	}

	return 0;
}
