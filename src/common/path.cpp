
#include "path.hpp"

zeq_path_t::zeq_path_t()
: std::string()
{
    
}

zeq_path_t::zeq_path_t(const std::string& path)
: std::string(path)
{
    normalize();
}

void zeq_path_t::normalize()
{
    // No backslashes
    for (char& c : *this)
    {
        if (c == '\\')
            c = '/';
    }
}

void zeq_path_t::ensureTrailingSlash()
{
    if (back() != '/')
        (*this) += '/';
}

std::string zeq_path_t::insertBeforeExtension(const std::string& str) const
{
    std::string ext     = getExtension(c_str());
    uint32_t extStart   = size() - ext.size() - 1;
    
    std::string ret = substr(0, extStart);
    
    ret += str;
    ret += '.';
    ret += ext;
    
    return ret;
}

std::string zeq_path_t::getFileName() const
{
    if (empty())
        return std::string();
    
    // Find last slash
    int start = (int)size();
    
    while (start != 0)
    {
        if ((*this)[start - 1] == '/')
            break;
        start--;
    }
    
    return substr((size_t)start);
}

std::string zeq_path_t::getFileNameNoExtension() const
{
    std::string str = getFileName();
    
    for (uint32_t i = 0; i < str.size(); i++)
    {
        char c = str[i];
        
        if (c == '.')
            return str.substr(0, i);
    }
    
    return str;
}

std::string zeq_path_t::getExtension(const std::string& str)
{
    if (str.empty())
        return std::string();
    
    // Find last slash, if any
    uint32_t searchFrom = 0;
    
    for (uint32_t i = 0; i < str.size(); i++)
    {
        char c = str[i];
        
        if (c == '/' || c == '\\')
            searchFrom = i + 1;
    }
    
    // Find first dot we run into from there
    for (uint32_t i = searchFrom; i < str.size(); i++)
    {
        char c = str[i];
        
        if (c == '.')
            return str.substr(i + 1);
    }
    
    return std::string();
}
