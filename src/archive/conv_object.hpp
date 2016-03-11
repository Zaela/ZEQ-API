
#ifndef _ZEQ_CONV_OBJECT_HPP_
#define _ZEQ_CONV_OBJECT_HPP_

#include "define.hpp"
#include <string>

class ConvObject
{
private:
    std::string m_name;

public:
    ConvObject() : m_name(std::string()) { }
    ConvObject(const std::string& name) : m_name(name) { }
    virtual ~ConvObject() { }
    
    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
};

#endif//_ZEQ_CONV_OBJECT_HPP_
