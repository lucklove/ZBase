/**************************************************************************
 * stringtokenizer.h                                                      * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

#include "explode.hh"
#include <string>
#include <vector>

namespace zbase {

class StringTokenizer
{
public:
	/**
 	 * \param str The string to be splited.
 	 * \param delim Dest string was splited by this charactor.
 	 */ 
	StringTokenizer(std::string str, char delim = ' ')
		:tokens(explode(str, delim)) {}
	
	StringTokenizer() = default;
	StringTokenizer(const StringTokenizer &) = default;
	
	/**
 	 * \brief Check if there are more tokens in the tokenizer.
 	 * \return If have, true, else, false.
 	 */ 
	bool hasMoreTokens() { return !tokens.empty(); }

	/**
 	 * \brief Get next token in the tokenizer.
 	 * \return Next token.
 	 */ 
	std::string nextToken()
	{
		std::string ret = tokens.front();
		tokens.erase(tokens.begin());
		return ret;
	}
private:
	std::vector<std::string> tokens;
};

}		/** namespace zbase */
