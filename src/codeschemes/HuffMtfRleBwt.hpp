/*
    Compression+Coding schema: Huffman(MTF(RLE(BWT(Input)))) (reverse operations for decompression)
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

#ifndef HUFFMTFRLEBWT_H
#define HUFFMTFRLEBWT_H
#include <string>
#include "../codecs/IEncoder.hpp"
#include "../codecs/BWT.hpp"
#include "../codecs/Huffman.hpp"
#include "../codecs/MTF.hpp"
#include "../codecs/RLE.hpp"

class HuffMtfRleBwt : public ICodec {
public:
    HuffMtfRleBwt(const std::string* filename, void (*debug_ptr)(std::string, int), int verbose_value,bool decompress);
    void compress(const std::string* outfile);
    void decompress(const std::string* output);
    virtual ~HuffMtfRleBwt();
private:
    void (*debug)(std::string, int);
    bool dcprs;
    std::string* bwtOut;  //.bwt     -  
    std::string* rleOut;  //.rle    .urle
    std::string* mtfOut;  //.mtf    .umtf
    std::string* huffOut; //.brmh -> .uhuf
    Huffman* huffman;
    MTF* mtf;
    RLE* rle;
    BWT* bwt;
};

#endif // HUFFBWT_H
