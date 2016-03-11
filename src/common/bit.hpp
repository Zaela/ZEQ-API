
#ifndef _ZEQ_BIT_HPP_
#define _ZEQ_BIT_HPP_

#include "define.hpp"

#define bitmask(n) ((1 << (n)) - 1)
#define bitmask64(n) ((1ULL << (n)) - 1ULL)
#define ispow2(n) ((n) && (((n) & ((n) - 1)) == 0))
#define bitwiserotate(x, n) (((x)<<(n)) | ((x)>>(-(int)(n)&(8*sizeof(x)-1))))
#define get_bit(val, n) (val & (1 << n))
#define get_bit64(val, n) (val & (1ULL << (n)))

#endif//_ZEQ_BIT_HPP_
