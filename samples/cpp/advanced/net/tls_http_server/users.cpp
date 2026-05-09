#include "users.h"

#include <charconv>
#include <format>
#include <string_view>
#include <utility>

UserStore::UserStore()
{
	m_users[1] = {1, "Alice", "alice@example.com"};
	m_users[2] = {2, "Bob", "bob@example.com"};
	m_next_id = 3;
}

std::vector<User> UserStore::all() const
{
	std::lock_guard lock(m_mtx);
	std::vector<User> out;
	out.reserve(m_users.size());
	for (const auto& kv : m_users) {
		out.push_back(kv.second);
	}
	return out;
}

const User* UserStore::find(std::uint32_t id) const
{
	std::lock_guard lock(m_mtx);
	const auto it = m_users.find(id);
	return (it != m_users.end()) ? &it->second : nullptr;
}

User UserStore::create(std::string name, std::string email)
{
	std::lock_guard lock(m_mtx);
	const std::uint32_t id = m_next_id++;
	User u{id, std::move(name), std::move(email)};
	m_users[id] = u;
	return u;
}

bool UserStore::remove(std::uint32_t id)
{
	std::lock_guard lock(m_mtx);
	return m_users.erase(id) > 0;
}

static std::uint32_t parse_id(std::string_view s)
{
	std::uint32_t id = 0;
	std::from_chars(s.data(), s.data() + s.size(), id);
	return id;
}

static std::string extract_field(std::string_view body, std::string_view key)
{
	const std::string needle = std::format(R"("{}":")", key);
	auto pos = body.find(needle);
	if (pos == std::string_view::npos) {
		return {};
	}
	pos += needle.size();
	const auto end = body.find('"', pos);
	if (end == std::string_view::npos) {
		return {};
	}
	return std::string(body.substr(pos, end - pos));
}

static HttpResponseType handle_list_users(const HttpRequest&, const RouteParams&, void* ctx)
{
	auto* store = static_cast<UserStore*>(ctx);
	return JSON::ok(ToJson(store->all()));
}

static HttpResponseType handle_create_user(const HttpRequest& req, const RouteParams&, void* ctx)
{
	auto* store = static_cast<UserStore*>(ctx);
	const std::string name = extract_field(req.body, "name");
	const std::string email = extract_field(req.body, "email");
	if (name.empty() || email.empty()) {
		return JSON::bad_request("missing name or email");
	}
	const User u = store->create(name, email);
	return JSON::created(to_json(u));
}

static HttpResponseType handle_get_user(const HttpRequest&, const RouteParams& p, void* ctx)
{
	auto* store = static_cast<UserStore*>(ctx);
	const std::uint32_t id = parse_id(p.get("id"));
	const User* u = store->find(id);
	if (u == nullptr) {
		return JSON::not_found("user not found");
	}
	return JSON::ok(to_json(*u));
}

static HttpResponseType handle_delete_user(const HttpRequest&, const RouteParams& p, void* ctx)
{
	auto* store = static_cast<UserStore*>(ctx);
	const std::uint32_t id = parse_id(p.get("id"));
	if (!store->remove(id)) {
		return JSON::not_found("user not found");
	}
	return HTTP::no_content();
}

void register_user_routes(Router& r, UserStore& store)
{
	r.add(HttpMethod::GET, "/users", handle_list_users, &store);
	r.add(HttpMethod::POST, "/users", handle_create_user, &store);
	r.add(HttpMethod::GET, "/users/:id", handle_get_user, &store);
	r.add(HttpMethod::DELETE, "/users/:id", handle_delete_user, &store);
}
