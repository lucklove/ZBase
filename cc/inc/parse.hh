/**************************************************************************
 * parse.h                                                      	  * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

#include <string>
#include <sstream>
#include <stdexcept>

namespace zbase {

/**
 * \param T Dest type.
 * \param str The string to be parsed.
 * \return Dest type's value in param str.
 */ 
template<typename T> T
parse(std::string str)
{
	if(str.size() == 0)
		throw(std::invalid_argument{str});
	std::istringstream s{str};
	T ret;
	s >> ret;
	if(!s.eof())
		throw(std::invalid_argument{str});
	return ret;
}	

} 		/** namespace zbase */
