#pragma once

#include "http.h"
#include "router.h"

#include <cstdint>
#include <format>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

struct User
{
	std::uint32_t id;
	std::string name;
	std::string email;
};

class UserStore
{
public:
	UserStore();

	std::vector<User> all() const;
	const User* find(std::uint32_t id) const;
	User create(std::string name, std::string email);
	bool remove(std::uint32_t id);

private:
	mutable std::mutex m_mtx;
	std::unordered_map<std::uint32_t, User> m_users;
	std::uint32_t m_next_id = 1;
};

template<typename T>
std::string to_json(const T& value);

template<>
inline std::string to_json<User>(const User& u)
{
	return std::format(R"({{"id":{},"name":"{}","email":"{}"}})", u.id, u.name, u.email);
}

template<typename T>
std::string ToJson(const std::vector<T>& vec)
{
	std::string out = "[";
	for (std::size_t i = 0; i < vec.size(); i++) {
		if (i > 0) {
			out += ',';
		}
		out += to_json(vec[i]);
	}
	out += ']';
	return out;
}

void register_user_routes(Router& r, UserStore& store);
