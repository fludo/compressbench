/*
    Compression+Coding schema: Huffman(BWT(Input)) (reverse operations for decompression)
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

#include "HuffBwt.hpp"

HuffBwt::HuffBwt(const std::string* filename, void (*debug_ptr)(std::string, int), int verbose_value,bool decompress) {
    this->dcprs = decompress;
        if (debug_ptr != NULL) {
        this->debug = debug_ptr;
    } else {
        this->debug = null_debug;
    }
    if (filename == NULL) {
        debug("Read from stdin not supported for chain-encoding",CRITICAL);
        this->bwt = NULL;
        this->huffman = NULL;
    }
    else {
        if (this->dcprs) {
	  //decompressing
	    
	    this->huffman = new Huffman(filename,debug_ptr,verbose_value);
            this->huffOut = new std::string(*filename);
	    
            this->huffOut->append(".uhuf");
            this->bwt = new BWT(this->huffOut,debug_ptr,verbose_value);
	    this->bwtOut = new std::string(filename->substr(0,filename->rfind(".",filename->size())));
	    this->bwtOut->append(".ubh");
	    
	    //removeExtension(this->bwtOut);
	    fprintf(stderr,"Decompressing...%s -> %s -> %s \n",filename->c_str(),this->huffOut->c_str(), this->bwtOut->c_str());
        }
        else {
	  //compressing
            this->bwt = new BWT(filename,debug_ptr,verbose_value);
            this->bwtOut = new std::string(*filename);
            this->bwtOut->append(".bwt");
            this->huffman = new Huffman(bwtOut,debug_ptr,verbose_value);
            this->huffOut = new std::string(*filename);
            this->huffOut->append(".bh");
	    fprintf(stderr,"Compressing...%s -> %s -> %s \n",filename->c_str(), this->bwtOut->c_str(),this->huffOut->c_str());
        }

    }
}

void HuffBwt::compress(const std::string* outfile) {
    this->bwt->compress(this->bwtOut);
    this->huffman->encode(this->huffOut);
}
void HuffBwt::decompress(const std::string* output) {
    this->huffman->decode(this->huffOut);
    this->bwt->decompress(this->bwtOut);
}

HuffBwt::~HuffBwt() {
    if (this->bwtOut)
    delete this->bwtOut;  //.bwt  -
    if (this->huffOut)
    delete this->huffOut; //.bh -> ^
    if (this->bwt)
    delete this->bwt;
    if(this->huffman)
    delete this->huffman;
}
