
#ifndef _ZEQ_PATH_HPP_
#define _ZEQ_PATH_HPP_

#include "define.hpp"
#include <string>

class zeq_path_t : public std::string
{
public:
    zeq_path_t();
    zeq_path_t(const std::string& path);
    ~zeq_path_t() { }

    zeq_path_t& operator=(const std::string& path) { return (*this = path); }

    const std::string& str() const { return *this; }

    void normalize();
    void ensureTrailingSlash();

    std::string insertBeforeExtension(const std::string& str) const;
    std::string getFileName() const;
    std::string getFileNameNoExtension() const;

    static std::string getExtension(const std::string& str);
};

#endif//_ZEQ_PATH_HPP_
