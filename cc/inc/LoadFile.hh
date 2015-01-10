#pragma once

#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include "exception.hh"

namespace zbase {

struct FileOpenError : public Exception {
	using Exception::Exception;
};

/**
 * \brief To load dest file in memory at once.
 * \param _data_type The type of data in dest file.
 */
template<typename _data_type>
class LoadFile {
public:
	/**
 	 * \brief Constructor.
 	 * \param filename File name of dest file.
 	 */ 
	explicit  LoadFile(std::string filename);
	~LoadFile()
	{
		if(data_ptr)
			std::free(data_ptr);
	}
	
	/**
 	 * \brief Get meta data in the file.
 	 * \return A pointer of _data_type, which point to the data.
 	 */ 
	_data_type *data() { return static_cast<_data_type*>(data_ptr); }
	
	/**
 	 * \brief Get the item number in the file.
 	 * \return The item number, the item is of _data_type.
 	 * \note This return the item number, not length by char.
 	 */ 
	unsigned int size() { return item_num; }
	
	/**
 	 * \brief Get the item at dest postion.
 	 * \param index Dest postion.
 	 * \return _data_type lvalue refrence of dest item.
 	 */ 
	_data_type& operator[](int index) 
	{ 
		return static_cast<_data_type *>(data_ptr)[index];
	}
	
	/**
 	 * \brief Convert to _data_type pointer.
 	 * \return _data_type pointer.
 	 * \see data().
 	 */ 
	operator _data_type*()
	{
		return static_cast<_data_type *>(data_ptr);
	}
	
private:
	void *data_ptr;			/**< meta data. */
	unsigned int item_num;		/**< item count. */
};

template<typename _data_type>
LoadFile<_data_type>::LoadFile(std::string file_name)
	:data_ptr(nullptr), item_num(0)
{
	std::ifstream file(file_name, std::ios::binary);
	if(!file)
		DEBUG_THROW(FileOpenError, "can't open dest file");
	file.seekg(0, std::ios::end);
	item_num = file.tellg() / sizeof(_data_type);
	file.seekg(0, std::ios::beg);
	data_ptr = std::malloc(item_num * sizeof(_data_type) + 1);
	if(data_ptr == nullptr) {
		file.close();
		throw std::bad_alloc();
	}
	memset(data_ptr, 0, item_num * sizeof(_data_type) + 1);
	file.read(static_cast<char *>(data_ptr), item_num * sizeof(_data_type)); 
	file.close();
}

}		/**< namespace zbase */
