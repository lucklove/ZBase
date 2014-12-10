/**************************************************************************
 * explode.h                                                              * 
 * Copyright (C) 2014 Joshua <gnu.crazier@gmail.com>                      *
 **************************************************************************/

#pragma once

#include <vector>

namespace zbase {

/**
 * \brief Split a container by a special item in it.
 * \param c The container to be splited.
 * \param i An item the container splited by.
 * \return A vector which stored sub containers after spliting.
 * \note
 * 	The _tContainer should have operator+=, which append item
 * 	to the end of that container.
 * 	The _tContainer should provide a default constructor which
 * 	construct a container which hase no items.
 * 	The _tContainer should also can be contained by vector.
 * 	The _tContainer should provide begin and end member functions.
 * 	The _tContainer should have size member function, which return
 * 	the number of items it contains.
 * 	The _tItem should have operator== which compare two items.
 */ 
template<typename _tContainer, typename _tItem> 
const std::vector<_tContainer> 
explode(const _tContainer& c, const _tItem& i)
{
	_tContainer buff;
	std::vector<_tContainer> v;
	
	for(auto n:c)
	{
		if(n != i) {
			buff += n; 
		} else if(n == i && buff.size() != 0) { 
				v.push_back(buff); 
				buff = _tContainer(); 
		}
	}
	if(buff.size() != 0) 
		v.push_back(buff);
	
	return v;
}

}			/**< namespace zbase. */
