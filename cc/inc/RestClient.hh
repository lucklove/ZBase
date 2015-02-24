#pragma once

#include <string>
#include <map>
#include <cstdlib>
#include <algorithm>

namespace zbase {

class RestClient
{
public:
	/**
	 * public data definitions
	 */
	using header_map_t = std::map<std::string, std::string>;

	/** response struct for queries */
	typedef struct {
		int code;
		std::string body;
		header_map_t headers;
	} response_t;

	/** struct used for uploading data */
	typedef struct {
		const char* data;
		size_t length;
	} upload_object_t;

	/** public methods */
	/** Auth */
	static void clearAuth();
	static void setAuth(const std::string& user,const std::string& password);
	/** HTTP GET */
	static response_t get(const std::string& url);
	/** HTTP POST */
	static response_t post(const std::string& url, const std::string& ctype, const std::string& data);
	/** HTTP PUT */
	static response_t put(const std::string& url, const std::string& ctype, const std::string& data);
	/** HTTP DELETE */
	static response_t del(const std::string& url);

private:
	/** writedata callback function */
	static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata);

	/** header callback function */
	static size_t header_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
    
	/** read callback function */
	static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
    
	static const char* user_agent;
	
	static std::string user_pass;

	/** trim from start */
    	static inline std::string &ltrim(std::string &s) {
      		s.erase(s.begin(), 
			std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
      		return s;
    	}

	/** trim from end */
    	static inline std::string &rtrim(std::string &s) {
      		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
			s.end());
      		return s;
    	}

	/** trim from both ends */
	static inline std::string &trim(std::string &s) {
 		return ltrim(rtrim(s));
	}
};

}		/**< namespace zbase */
