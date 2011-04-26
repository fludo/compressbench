/*
    Burrows-Wheeler Transform class
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

#ifndef _BWT_H
#define	_BWT_H
#include <string>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <set>
#include <bitset> //for bitstyle printing
#include <cmath>
#include "../defines.hpp"
#include "ICodec.hpp"
//#include "../defines.h"

// depending on platform, integer are coded on 16 bits
// According to Manzini's paper, bzip2 use block size from 100Kb to 900Kb
// Try with very large block if O.S. allows it
// Based on benchmark, 128k would be a good choice for most of the bench files.
#define BLOCKSIZE  128000 // has to be geq than 3
/*
#if ( INT_MAX <= 0x7F )
#define BLOCKSIZE 32000
#else
#define BLOCKSIZE 900000
#endif
*/
typedef unsigned char* BYTE_PTR;
static std::vector<unsigned char> baseblock;
static unsigned long curr_length;
static unsigned char buffer[BLOCKSIZE];
static unsigned char doubleBuffer[2*BLOCKSIZE];
class BWT : public ICodec {
public:
    BWT(const std::string* infilename, void (*debug_ptr)(std::string, int), int verbose_value);
    //BWT(const BWT& orig);
    void compress(const std::string*outfile);
    void decompress(const std::string* output);
    virtual ~BWT();


private:
    // general settings
    void (*debug)(std::string, int);
    int verbose;
    std::string* filename;
    // private members for compression part




    inline unsigned int iLog2(unsigned int value);


    //Inspired by : http://www.java2s.com/Tutorial/Cpp/0380__set-multiset/setcomparison.htm
    // sorting class for STL set usage

    class RuntimeCmp {
    public:
        /*
         * As the block suffix sorting basic version was too expensive, I now use
         * pointers.
         * The double buffer is used to allow complete comparison.
         *
         */
        int operator()( const unsigned char *p1,
                        const unsigned char *p2 ) const
        {
            int result = memcmp( doubleBuffer-(buffer - p1), doubleBuffer-(buffer - p2), curr_length );
            if ( result < 0 )
                return 1;
            if ( result > 0 )
                return 0;
            return p1 > p2;
        }
    };

};

#endif	/* _BWT_H */

