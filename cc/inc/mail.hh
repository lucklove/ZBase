#pragma once

#include <string>

namespace mdd {

bool sendMail(const std::string& from, const std::string& to,
	const std::string& password, const std::string& server,
	const std::string& port, const std::string& subject, const std::string& body);

}
