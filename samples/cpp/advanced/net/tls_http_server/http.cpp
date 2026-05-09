#include "http.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <ranges>
#include <string>

HttpMethod parse_method(std::string_view s)
{
	if (s == "GET") {
		return HttpMethod::GET;
	}
	if (s == "POST") {
		return HttpMethod::POST;
	}
	if (s == "DELETE") {
		return HttpMethod::DELETE;
	}
	return HttpMethod::UNKNOWN;
}

std::string_view method_name(HttpMethod m)
{
	switch (m) {
		case HttpMethod::GET:    return "GET";
		case HttpMethod::POST:   return "POST";
		case HttpMethod::DELETE: return "DELETE";
		default:                 break;
	}
	return "NULL";
}

static std::string ToLower(std::string s)
{
	std::ranges::transform(s, s.begin(), [](unsigned char c)
	{
		return static_cast<char>(std::tolower(c));
	});
	return s;
}

static std::string_view trim(std::string_view s)
{
	while (!s.empty() && (s.front() == ' ' || s.front() == '\t' || s.front() == '\r' || s.front() == '\n')) {
		s.remove_prefix(1);
	}
	while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r' || s.back() == '\n')) {
		s.remove_suffix(1);
	}
	return s;
}

bool parse_request(std::string_view raw, HttpRequest& out)
{
	const auto header_end = raw.find("\r\n\r\n");
	if (header_end == std::string_view::npos) {
		return false;
	}

	std::string_view header_section = raw.substr(0, header_end);
	std::string_view body_section = raw.substr(header_end + 4);

	const auto line_end = header_section.find("\r\n");
	if (line_end == std::string_view::npos) {
		return false;
	}

	std::string_view request_line = header_section.substr(0, line_end);
	header_section.remove_prefix(line_end + 2);

	const auto m_end = request_line.find(' ');
	if (m_end == std::string_view::npos) {
		return false;
	}
	out.method = parse_method(request_line.substr(0, m_end));
	request_line.remove_prefix(m_end + 1);

	const auto p_end = request_line.find(' ');
	if (p_end == std::string_view::npos) {
		return false;
	}
	out.path = std::string(request_line.substr(0, p_end));
	out.version = std::string(trim(request_line.substr(p_end + 1)));

	while (!header_section.empty()) {
		const auto eol = header_section.find("\r\n");
		std::string_view line = (eol == std::string_view::npos) ? header_section : header_section.substr(0, eol);
		if (eol != std::string_view::npos) {
			header_section.remove_prefix(eol + 2);
		} else {
			header_section = {};
		}

		const auto colon = line.find(':');
		if (colon == std::string_view::npos) {
			continue;
		}
		out.headers[ToLower(std::string(trim(line.substr(0, colon))))] = std::string(trim(line.substr(colon + 1)));
	}

	if (auto it = out.headers.find("content-length"); it != out.headers.end()) {
		std::size_t len = 0;
		std::from_chars(it->second.data(), it->second.data() + it->second.size(), len);
		if (len > 0 && len <= body_section.size()) {
			out.body = std::string(body_section.substr(0, len));
		}
	}

	return true;
}

bool keep_alive(const HttpRequest& req)
{
	if (auto it = req.headers.find("connection"); it != req.headers.end()) {
		return ToLower(it->second) != "close";
	}
	return req.version == "HTTP/1.1";
}
