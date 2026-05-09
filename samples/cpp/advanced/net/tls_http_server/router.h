#pragma once

#include "http.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct RouteParams
{
	std::unordered_map<std::string, std::string> vars;

	std::string_view get(std::string_view key) const
	{
		const auto it = vars.find(std::string(key));
		return (it != vars.end()) ? std::string_view(it->second) : std::string_view{};
	}
};

using HandlerFn = HttpResponseType (*)(const HttpRequest&, const RouteParams&, void*);

struct RouteEntry
{
	HttpMethod method;
	std::vector<std::string> segments;
	HandlerFn handler;
	void* ctx;
};

class Router
{
public:
	void add(HttpMethod method, std::string_view pattern, HandlerFn handler, void* ctx = nullptr);

	HttpResponseType dispatch(const HttpRequest& req) const;

private:
	static std::vector<std::string> split_path(std::string_view path);
	static bool match(const std::vector<std::string>& pattern, const std::vector<std::string>& actual, RouteParams& out);

private:
	std::vector<RouteEntry> m_routes;
};
