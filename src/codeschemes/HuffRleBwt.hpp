/*
    Compression+Coding schema: Huffman(RLE(BWT(Input))) (reverse operations for decompression)
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

#ifndef HUFFRLEBWT_H
#define HUFFRLEBWT_H
#include <string>
#include "../codecs/IEncoder.hpp"
#include "../codecs/BWT.hpp"
#include "../codecs/Huffman.hpp"
#include "../codecs/RLE.hpp"

class HuffRleBwt : public ICodec {
public:
    HuffRleBwt(const std::string* filename, void (*debug_ptr)(std::string, int), int verbose_value,bool decompress);
    void compress(const std::string* outfile);
    void decompress(const std::string* output);
    virtual ~HuffRleBwt();
private:
    void (*debug)(std::string, int);
    bool dcprs;
    std::string* bwtOut;  //.bwt   -
    std::string* rleOut;  //.rle   .urle
    std::string* huffOut; //.brh -> .uhuf
    Huffman* huffman;
    BWT* bwt;
    RLE* rle;
};

#endif // HUFFBWT_H
