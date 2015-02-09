#include "http.hh"

namespace zbase {

static HttpResult
get_response(boost::asio::ip::tcp::socket& socket)
{
	boost::asio::streambuf response;
	boost::asio::read_until(socket, response, "\r\n");
	std::istream response_stream(&response);
	std::string http_version;
	response_stream >> http_version;
	unsigned int status_code;
	response_stream >> status_code;
	std::string status_message;
	std::getline(response_stream, status_message);
	if(!response_stream || http_version.substr(0, 5) != "HTTP/")
		return HttpResult::BadResponse("Invalid response");
	if(status_code != 200)
		return HttpResult::BadResponse(status_code);
	
	boost::asio::read_until(socket, response, "\r\n\r\n");
	
	std::string header;
	HttpResult result;
	while(std::getline(response_stream, header) && header != "\r")
		result.addHeader(header);
	std::ostringstream os;
	if(response.size() > 0)
		os << &response;
	
	boost::system::error_code error;
	while(boost::asio::read(socket, response, 
			boost::asio::transfer_at_least(1), error))
		os << &response;
	if(error != boost::asio::error::eof)
		throw boost::system::system_error(error);
	result.setContent(os.str());
	return result;
}

static boost::asio::ip::tcp::socket
connect(boost::asio::io_service& io_service, const std::string& host, const std::string& service)
{
	using tcp = boost::asio::ip::tcp;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(host, service);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;
	tcp::socket socket(io_service);
	boost::system::error_code error = boost::asio::error::host_not_found;
	while(error && endpoint_iterator != end) {
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}
	if(error)
		throw boost::system::system_error(error);

	return socket;
}

HttpResult
Http::syncGet(const std::string& host, const std::string& path, const std::string& service)
{
	tcp::socket socket = connect(io_service, host, service);	
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream << "GET " << path << " HTTP/1.0\r\n";
	request_stream << "Host: " << host << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: close\r\n\r\n";
	
	boost::asio::write(socket, request);
	
	return get_response(socket);	
}

HttpResult
Http::syncPost(const std::string& host, const std::string& data,
			const std::string& path, const std::string& service)
{
	tcp::socket socket = connect(io_service, host, service);
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream << "POST " << path << " HTTP/1.0\r\n";
	request_stream << "Host: " << host << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Content-Length: " << data.size() << "\r\n";
	request_stream << "Content-Type: application/x-www-form-urlencoded\r\n";
	request_stream << "Connection: close\r\n\r\n";
	request_stream << data;

	boost::asio::write(socket, request);

	return get_response(socket);
}

}	/**< namespace zbase */
