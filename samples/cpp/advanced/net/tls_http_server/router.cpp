#include "router.h"

void Router::add(const HttpMethod method, std::string_view pattern, const HandlerFn handler, void* const ctx)
{
	m_routes.push_back({method, split_path(pattern), handler, ctx});
}

HttpResponseType Router::dispatch(const HttpRequest& req) const
{
	const std::vector<std::string> actual = split_path(req.path);
	bool path_matched = false;

	for (const RouteEntry& entry : m_routes) {
		RouteParams params;
		if (!match(entry.segments, actual, params)) {
			continue;
		}
		path_matched = true;
		if (entry.method != req.method) {
			continue;
		}
		return entry.handler(req, params, entry.ctx);
	}

	if (path_matched) {
		return JSON::method_not_allowed();
	}
	return JSON::not_found(req.path);
}

std::vector<std::string> Router::split_path(std::string_view path)
{
	std::vector<std::string> parts;
	std::size_t start = 0;
	if (!path.empty() && path[0] == '/') {
		start = 1;
	}

	while (start < path.size()) {
		auto end = path.find('/', start);
		if (end == std::string_view::npos) {
			end = path.size();
		}
		parts.emplace_back(path.substr(start, end - start));
		start = end + 1;
	}
	return parts;
}

bool Router::match(const std::vector<std::string>& pattern, const std::vector<std::string>& actual, RouteParams& out)
{
	if (pattern.size() != actual.size()) {
		return false;
	}

	for (std::size_t i = 0; i < pattern.size(); ++i) {
		if (pattern[i].starts_with(':')) {
			out.vars[pattern[i].substr(1)] = actual[i];
		} else if (pattern[i] != actual[i]) {
			return false;
		}
	}

	return true;
}
