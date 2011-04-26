/*
    Huffman Coding implementation: Binary i/o utility class
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

#ifndef _BINARYIO_H
#define	_BINARYIO_H
/*
 * This class will be used to write the bits of the encoded file, not
 * the header....
 *
 */

#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
class BinaryIO {
public:
    BinaryIO(const char* filename,const char mode);
    BinaryIO(const char mode);
    BinaryIO(const BinaryIO& orig);
    virtual ~BinaryIO();
    int flush();
    int appendBit(unsigned char bit);
    int appendBits(std::vector<unsigned char> bits);
    unsigned char readBit();
    unsigned char readByte();
    void readNext();
    bool eof();

private:
    std::string* filename;
    bool write; //true = write, false = read
    unsigned char buffer;
    unsigned char currBit_ptr;// ptr on the next bit to output [76543210]
    bool bufferIsEmpty;

};

#endif	/* _BINARYIO_H */

