#pragma once

#include <map>
#include <tuple>
#include <initializer_list>

namespace zbase {

template<typename _Element_t, typename... _Indexs_t>
class MultiIndexMap {
public:
	MultiIndexMap() = default;
	MultiIndexMap(const MultiIndexMap&) = default;
	MultiIndexMap(MultiIndexMap&&) = default;
	~MultiIndexMap() = default;
		
	_Element_t& operator() (_Indexs_t...);
private:
	std::map<std::tuple<_Indexs_t...>, _Element_t> _map;
};

template<typename _Element_t, typename... _Indexs_t>
inline _Element_t&
MultiIndexMap<_Element_t, _Indexs_t...>::operator() (_Indexs_t... indexs)
{
	return _map[std::make_tuple(indexs...)];
}

}		/**< namespace zbase */
