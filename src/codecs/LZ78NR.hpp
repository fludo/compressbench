/*
    LZ78 basic algorithm implementation with varying dictionnary size, no reset of dictionnary
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

#ifndef _LZ78NR_H
#define	_LZ78NR_H
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
namespace LZNR
{
// Valid pairs : 127,1 | 32767,2 | 8388607,3   formula : MAX_DICSIZE = (2^(numbits-1)-1)
#define NDICT_NUMBYTE 2

#if NDICT_NUMBYTE == 3
#define NMAX_DICSIZE 8388607
#elif NDICT_NUMBYTE == 2
#define NMAX_DICSIZE 32767
#else
#define NMAX_DICSIZE 127
#endif


#if NDICT_NUMBYTE == 1
#define NZERO 0x00
#define NMASK 0x80
#define NMATCH 0x80
#define NMISS  0x0F
#define NDICT_FULL 0x08
#define NEOFILE 0x7F // 01111111b
#define NUNSET_MASK 0x7F
#endif

#if NDICT_NUMBYTE == 2
#define NZERO 0x0000
#define NMASK 0x8000
#define NMATCH 0x8000
#define NMISS  0x0FFF
#define NDICT_FULL 0x08FF
#define NEOFILE 0x7FFF
#define NUNSET_MASK 0x7FFF
#endif

#if NDICT_NUMBYTE == 3
#define NZERO 0x000000
#define NMASK 0x800000
#define NMATCH 0x800000
#define NMISS  0x0FFFFF
#define NDICT_FULL 0x08FFFF
#define NEOFILE 0x7FFFFF
#define NUNSET_MASK 0x7FFFFF
#endif



// per byte operation
#define NSET_MATCH(x)    (x | NMATCH)
#define NSET_MISS(x)    (x | NMISS)

#define NCLEAN_FLAG(x) (x & NUNSET_MASK)

#define NIS_MATCH(x)      ((x & NMASK) != NZERO)


class LZ78NR : public ICodec {
public:
    LZ78NR(const std::string* filename, void (*debug_ptr)(std::string, int), int verbose_value);
    //LZ78(const LZ78& orig);
    void compress(const std::string* outfile);
    void decompress(const std::string* output);
    virtual ~LZ78NR();
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

