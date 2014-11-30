#pragma once

#include <vector>
#include <string>

namespace zbase {

/**
 * \brief Split a string.
 * \param s The string to be splited.
 * \param c A charactor the string splited by.
 * \return A vector<string> which stored strings after spliting.
 */ 
const std::vector<std::string> explode(const std::string& s, const char& c);

} 			/**< namespace zbase. */
