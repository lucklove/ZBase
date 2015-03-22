#include "mail.hh"
#include <string>
#include <algorithm>
#include <boost/asio.hpp>
#include <sstream>
#include <iostream>

namespace mdd {

static std::string
base64Encode(const std::string& input)
{
        ::base64_t b64 = ::makeBase64Type(input.c_str(), input.size());
        ::base64_t enc = ::base64Encode(b64);
        std::string ret(static_cast<char *>(::getBase64Ptr(enc)), ::getBase64Len(enc));
        ::destroyBase64Type(b64);
        ::destroyBase64Type(enc);
        return ret;
}

static int
get_code(boost::asio::streambuf& buf)
{
	std::stringstream ss;
	ss << &buf;
	int code;
	ss >> code;
	return code;
}

bool
sendMail(const std::string& from, const std::string& to, 
	const std::string& password, const std::string& server,
	const std::string& port, const std::string& subject, const std::string& body)
{
	using boost::asio::ip::tcp;
	using namespace boost::asio;
	
	auto&& p = std::find(from.cbegin(), from.cend(), '@');
	if(p == from.cend())
		return false;
	
	std::string username(from.cbegin(), p);

	boost::asio::io_service service;
	tcp::resolver resolver(service);
	tcp::resolver::query query(ip::tcp::v4(), server, port);
	tcp::resolver::iterator iter = resolver.resolve(query);

	ip::tcp::socket sock(service);

	sock.connect(*iter);
	streambuf buf;

	read_until(sock, buf, "\n");
	if(get_code(buf) != 220)
		return false;

	write(sock, buffer("HELO " + server + "\r\n"));

	read_until(sock, buf, "\n");
	if(get_code(buf) != 250)
		return false;

	write(sock, buffer("AUTH LOGIN\r\n"));
	
	read_until(sock, buf, "\n");
	if(get_code(buf) != 334)
		return false;
	
	write(sock, buffer(base64Encode(username) + "\r\n"));

	read_until(sock, buf, "\n");
	if(get_code(buf) != 334)
		return false;
	
	write(sock, buffer(base64Encode(password) + "\r\n"));

	read_until(sock, buf, "\n");
	if(get_code(buf) != 235)
		return false;
	write(sock, buffer("MAIL FROM: <" + from + ">\r\n"));

	read_until(sock, buf, "\n");
	if(get_code(buf) != 250) {
		/**< FIXME: 这里需要尝试两次，原因暂时不知道. */
		write(sock, buffer("MAIL FROM: <" + from + ">\r\n"));

		read_until(sock, buf, "\n");
		if(get_code(buf) != 250)
			return false;
	}

	write(sock, buffer("RCPT TO: <" + to + ">\r\n"));

	read_until(sock, buf, "\n");
	if(get_code(buf) != 250)
		return false;

	write(sock, buffer("data\r\n"));
	
	read_until(sock, buf, "\n");
	if(get_code(buf) != 354)
		return false;

	write(sock, buffer("subject:" + subject + "\r\n"));
	write(sock, buffer("\r\n" + body + "\r\n"));
	write(sock, buffer(".\r\n"));

	read_until(sock, buf, "\r\n");
	if(get_code(buf) != 250)
		return false;

	write(sock, buffer("quit\r\n"));

	sock.close();

	return true;
}

}	/**< namespace mdd */
