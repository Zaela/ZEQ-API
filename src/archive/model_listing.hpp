
#ifndef _ZEQ_MODEL_LISTING_HPP_
#define _ZEQ_MODEL_LISTING_HPP_

#include "define.hpp"

struct ModelListing
{
    const char* name;
    uint8_t     type;
    bool        loaded;
    
    ModelListing(const char* _name, int _type) : name(_name), type((uint8_t)_type), loaded(false) { }
};

#endif//_ZEQ_MODEL_LISTING_HPP_
