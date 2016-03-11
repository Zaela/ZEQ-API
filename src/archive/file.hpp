
#ifndef _ZEQ_FILE_HPP_
#define _ZEQ_FILE_HPP_

#include "define.hpp"
#include <string>

class File
{
public:
    static bool     exists(const std::string& path);
    static byte*    openBinary(const std::string& path, uint32_t& len);
};

#endif//_ZEQ_FILE_HPP_
