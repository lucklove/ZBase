/**************************************************************************
 * parse.h                                                      	  * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

#include <string>
#include <sstream>
#include "exception.hh"

namespace zbase {

struct ParseError : public Exception {
	using Exception::Exception;
};

/**
 * \param T Dest type.
 * \param str The string to be parsed.
 * \return Dest type's value in param str.
 */ 
template<typename T> T
parse(std::string str)
{
	if(str.size() == 0)
		DEBUG_THROW(ParseError, "can't parse to dest type");
	std::istringstream s{str};
	T ret;
	s >> ret;
	if(!s.eof())
		DEBUG_THROW(ParseError, "can't parse to dest type");
	return ret;
}	

} 		/**< namespace zbase */
