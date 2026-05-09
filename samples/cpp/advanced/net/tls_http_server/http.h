#pragma once

#include <concepts>
#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <variant>

enum class HttpMethod : std::uint8_t
{
	UNKNOWN,
	GET,
	POST,
	DELETE,
};

using HeaderMap = std::unordered_map<std::string, std::string>;

struct HttpRequest
{
	HttpMethod method = HttpMethod::UNKNOWN;
	std::string path;
	std::string version;
	HeaderMap headers;
	std::string body;
};

HttpMethod parse_method(std::string_view s);
std::string_view method_name(HttpMethod m);
bool parse_request(std::string_view raw, HttpRequest& out);
bool keep_alive(const HttpRequest& req);

template<typename T>
concept TResponseBody = requires(const T& t) {
	{ t.bytes() } -> std::convertible_to<std::string_view>;
	{ T::content_type() } -> std::convertible_to<std::string_view>;
};

struct JsonBody
{
	std::string data;
	std::string_view bytes() const noexcept { return data; }
	static constexpr std::string_view content_type() noexcept { return "application/json"; }
};

struct TextBody
{
	std::string data;
	std::string_view bytes() const noexcept { return data; }
	static constexpr std::string_view content_type() noexcept { return "text/plain; charset=utf-8"; }
};

struct HtmlBody
{
	std::string data;
	std::string_view bytes() const noexcept { return data; }
	static constexpr std::string_view content_type() noexcept { return "text/html; charset=utf-8"; }
};

struct EmptyBody
{
	std::string_view bytes() const noexcept { return {}; }
	static constexpr std::string_view content_type() noexcept { return "application/octet-stream"; }
};

template<TResponseBody Body>
class HttpResponse
{
public:
	int status = 200;
	std::string status_text = "OK";
	HeaderMap headers;
	Body body;

	HttpResponse() = default;
	HttpResponse(const int s, std::string t, Body b)
		: status(s)
		, status_text(std::move(t))
		, body(std::move(b))
	{
	}

	void set_header(std::string key, std::string value)
	{
		headers.insert_or_assign(std::move(key), std::move(value));
	}

	template<typename K, typename V>
	HttpResponse& with(K&& key, V&& value)
	{
		headers.insert_or_assign(std::forward<K>(key), std::forward<V>(value));
		return *this;
	}

	[[nodiscard]] std::string serialize() const
	{
		const std::string_view body_view = body.bytes();
		std::string out = std::format("HTTP/1.1 {} {}\r\n", status, status_text);
		out += std::format("Content-Type: {}\r\n", Body::content_type());
		out += std::format("Content-Length: {}\r\n", body_view.size());
		for (const auto& [k, v] : headers) {
			out += std::format("{}: {}\r\n", k, v);
		}
		out += "\r\n";
		out += body_view;
		return out;
	}
};

using HttpResponseType = std::variant<
	HttpResponse<JsonBody>,
	HttpResponse<TextBody>,
	HttpResponse<HtmlBody>,
	HttpResponse<EmptyBody>>;

template<TResponseBody Body>
HttpResponse<Body> make_response(int status, std::string status_text, Body body)
{
	return HttpResponse<Body>{status, std::move(status_text), std::move(body)};
}

namespace JSON {
	inline HttpResponseType ok(std::string body)
	{
		return make_response(200, "OK", JsonBody{std::move(body)});
	}

	inline HttpResponseType created(std::string body)
	{
		return make_response(201, "Created", JsonBody{std::move(body)});
	}

	inline HttpResponseType not_found(std::string_view detail = "not found")
	{
		return make_response(404, "Not Found", JsonBody{std::format(R"({{"error":"{}"}})", detail)});
	}

	inline HttpResponseType bad_request(std::string_view detail = "bad request")
	{
		return make_response(400, "Bad Request", JsonBody{std::format(R"({{"error":"{}"}})", detail)});
	}

	inline HttpResponseType method_not_allowed()
	{
		return make_response(405, "Method Not Allowed", JsonBody{R"({"error":"method not allowed"})"});
	}
}

namespace HTTP {
	inline HttpResponseType no_content()
	{
		return make_response(204, "No Content", EmptyBody{});
	}

	inline HttpResponseType text(const int status, std::string body)
	{
		return make_response(status, (status == 200) ? "OK" : "", TextBody{std::move(body)});
	}

	inline HttpResponseType html(const int status, std::string body)
	{
		return make_response(status, (status == 200) ? "OK" : "", HtmlBody{std::move(body)});
	}
}
