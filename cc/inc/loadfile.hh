#pragma once

#include <fstream>
#include <cstdlib>
#include <string>
#include <ios>
#include <new>

namespace zbase {

template<typename _data_type>
class LoadFile {
public:
	explicit  LoadFile(std::string);
	~LoadFile()
	{
		if(data_ptr)
			std::free(data_ptr);
	}			
	_data_type *data() { return static_cast<_data_type*>(data_ptr); }
	unsigned int len() { return item_num; }
	
private:
	void *data_ptr;
	unsigned int item_num;
};

template<typename _data_type>
LoadFile<_data_type>::LoadFile(std::string file_name)
	:data_ptr(nullptr), item_num(0)
{
	std::ifstream file(file_name, std::ios::binary);
	if(!file)
		throw std::ios_base::failure("can't open file");
	file.seekg(0, std::ios::end);
	item_num = file.tellg() / sizeof(_data_type);
	file.seekg(0, std::ios::beg);
	data_ptr = std::malloc(item_num * sizeof(_data_type));
	if(data_ptr == nullptr) {
		file.close();
		throw std::bad_alloc();
	}
	file.read(static_cast<char *>(data_ptr), item_num * sizeof(_data_type)); 
	file.close();
}

}		/**< namespace zbase */
