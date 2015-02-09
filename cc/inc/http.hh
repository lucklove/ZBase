#include <string>
#include <vector>
#include <boost/asio.hpp>

namespace zbase {

class HttpResult {
public:
	HttpResult() : code(200) {}
	HttpResult(const std::string& msg) : error_msg(msg) {}
	HttpResult(unsigned int code) : code(code) {}

	operator bool() { return code == 200; }
	operator std::string() { return content; }
	void addHeader(const std::string& h) { header.push_back(h); }
	void setContent(const std::string& c) { content = c; }
	std::string getContent() { return content; }
	std::string getError() { return error_msg; }
	unsigned int getCode() { return code; }
	static HttpResult BadResponse(const std::string& msg) { return HttpResult(msg); }
	static HttpResult BadResponse(unsigned int code) { return HttpResult(code); }
private:
	unsigned int code;
	std::string error_msg;
	std::vector<std::string> header;
	std::string content;
};

class Http {
public:
	HttpResult syncGet(const std::string& host, const std::string& path = "/", const std::string& service = "http");
	HttpResult syncPost(const std::string& host, const std::string& data, 
			const std::string& path = "/", const std::string& service = "http");
	
private:
	using tcp = boost::asio::ip::tcp;
	boost::asio::io_service io_service;
};

} 	/**< namespace zbase */	
