/*
    Huffman Coding implementation
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

#ifndef _HUFFMAN_H
#define	_HUFFMAN_H
#include <string>
#include <set>
#include <map>
#include <assert.h>
#include "../defines.hpp"
#include "stdio.h"
#include "IEncoder.hpp"
#include "huffman/HNode.hpp"
#include "huffman/BinaryIO.hpp"
#include "limits.h"
#define NDEBUG 1;

class Huffman : public IEncoder {
public:
    Huffman(const std::string* infilename, void (*debug_ptr)(std::string, int), int verbose_value);
    Huffman(const Huffman& orig);
    virtual ~Huffman();
    void encode(const std::string*outfile);
    void decode(const std::string*outfile);
private:
    void (*debug)(std::string, int);
    int verbose;
    std::string* filename;

    class HNodeSorting {
    public:
        HNodeSorting() {
        }
        bool operator() (HNode* n1, HNode* n2)const{
            return ((n1->probability()) < (n2->probability()) || n1<n2);
        }
    };
    
};

#endif	/* _HUFFMAN_H */

