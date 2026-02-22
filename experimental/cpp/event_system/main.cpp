#include "core/core.h"
#include "ring_buffer.h"

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
	buf.put(10);
	buf.put(11);
	buf.put(12);

	int x = 0;
	buf.get(x);
	buf.get(x);
	buf.get(x);

	buf.put(30);
	buf.put(31);
	buf.put(32);

	buf.put_front(20);
	buf.get(x);

	buf.put_front(21);
	buf.put_front(22);
}
