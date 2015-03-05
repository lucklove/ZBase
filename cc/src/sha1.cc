#include "sha1.hh"
#include <cstdint>
#include <boost/uuid/sha1.hpp>
#include <sstream>
#include <iomanip>

namespace zbase {

std::string
sha1Encode(std::string param)
{
	boost::uuids::detail::sha1 sha;
	sha.process_bytes(param.c_str(), param.size());
	
	uint32_t digest[5];
	sha.get_digest(digest);
		
	std::ostringstream os;
	os << std::setfill('0');
	for(size_t i = 0; i < 5; ++i)
		os << std::setw(8) << std::hex << digest[i];
	
	return os.str();
}

}	/**< namespace zbase */
