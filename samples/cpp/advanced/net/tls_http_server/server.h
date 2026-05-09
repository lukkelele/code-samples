#pragma once

#include "router.h"
#include "socket.h"
#include "transport.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

class HttpServer
{
public:
	HttpServer(Router& r, TransportFactory factory, void* factory_ctx)
		: m_router(r)
		, m_factory(factory)
		, m_factory_ctx(factory_ctx)
	{
	}

	bool listen(std::string_view host, std::uint16_t port);
	void run();
	void stop() noexcept;

private:
	void handle_connection(std::unique_ptr<ITransport> t, std::string peer_addr) const;

private:
	Router& m_router;
	TransportFactory m_factory;
	void* m_factory_ctx;
	Socket m_listener;
	std::atomic_bool m_running = false;
};
