/*
    LZ78 basic algorithm implementation with varying dictionnary size
    Copyright (C) 2010  Ludovic Favre <ludovic.favre@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef _LZ78_H
#define	_LZ78_H
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <map>
#include <bitset> //for bitstyle printing
#include "../defines.hpp"
#include "ICodec.hpp"
namespace LZ{
// Valid pairs : 127,1 | 32767,2 | 8388607,3   formula : MAX_DICSIZE = (2^(numbits-1)-1)
#define DICT_NUMBYTE 2//2 is the best for calgary corpus, see report

#if DICT_NUMBYTE == 3
#define MAX_DICSIZE 8388607
#elif DICT_NUMBYTE == 2
#define MAX_DICSIZE 32767
#else
#define MAX_DICSIZE 127
#endif


#if DICT_NUMBYTE == 1
#define ZERO 0x00
#define MASK 0x80
#define MATCH 0x80
#define MISS  0x0F
#define DICT_FULL 0x08
#define EOFILE 0x7F // 01111111b
#define UNSET_MASK 0x7F
#endif

#if DICT_NUMBYTE == 2
#define ZERO 0x0000
#define MASK 0x8000
#define MATCH 0x8000
#define MISS  0x0FFF
#define DICT_FULL 0x08FF
#define EOFILE 0x7FFF
#define UNSET_MASK 0x7FFF
#endif

#if DICT_NUMBYTE == 3
#define ZERO 0x000000
#define MASK 0x800000
#define MATCH 0x800000
#define MISS  0x0FFFFF
#define DICT_FULL 0x08FFFF
#define EOFILE 0x7FFFFF
#define UNSET_MASK 0x7FFFFF
#endif



// per byte operation
#define SET_MATCH(x)    (x | MATCH)
#define SET_MISS(x)    (x | MISS)

#define CLEAN_FLAG(x) (x & UNSET_MASK)

#define IS_MATCH(x)      ((x & MASK) != ZERO)


class LZ78 : public ICodec {
public:
    LZ78(const std::string* filename, void (*debug_ptr)(std::string, int), int verbose_value);
    //LZ78(const LZ78& orig);
    void compress(const std::string* outfile);
    void decompress(const std::string* output);
    virtual ~LZ78();
private:
    /* Fields */
    std::map<std::vector<unsigned char>, unsigned long>* dict;
    std::map<unsigned long, std::vector<unsigned char> >* decode_dict;
    unsigned long nextIndex;
    std::string* filename;
    void (*debug)(std::string, int);
    int verbose;

    /* Methods */
    unsigned long isInDictionary(std::vector<unsigned char> entry);
    std::vector<unsigned char> getEntry(unsigned long index);
    void writeDictToByte(unsigned long value);
};
}
#endif	/* _LZ78_H */

