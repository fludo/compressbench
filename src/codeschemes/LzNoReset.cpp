/*
    Compression+Coding schema: LZ78(Input) (reverse operations for decompression) without dictionary reset
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

#include "LzNoReset.hpp"
LzNoReset::LzNoReset(const std::string* filename, void (*debug_ptr)(std::string, int), int verbose_value,bool decompress) {
    this->dcprs = decompress;
        if (debug_ptr != NULL) {
        this->debug = debug_ptr;
    } else {
        this->debug = null_debug;
    }
    if (filename == NULL) {
        debug("Read from stdin not supported for chain-encoding",CRITICAL);
        this->lz = NULL;
    }
    else {
        if (this->dcprs) {
	  //decompressing
	    
	    this->lz = new LZ78NR(filename,debug_ptr,verbose_value);
	    
	    this->lzOut = new std::string(filename->substr(0,filename->rfind(".",filename->size())));
	    this->lzOut->append(".ulz78nr");
	    
	    //removeExtension(this->bwtOut);
	    fprintf(stderr,"Decompressing...%s -> %s \n",filename->c_str(),this->lzOut->c_str());
        }
        else {
	  //compressing
            this->lz = new LZ78NR(filename,debug_ptr,verbose_value);
            this->lzOut = new std::string(*filename);
            this->lzOut->append(".lz78nr");
	    fprintf(stderr,"Compressing...%s -> %s \n",filename->c_str(), this->lzOut->c_str());
        }

    }
}

void LzNoReset::compress(const std::string* outfile) {
    this->lz->compress(this->lzOut);
}
void LzNoReset::decompress(const std::string* output) {
    this->lz->decompress(this->lzOut);
}

LzNoReset::~LzNoReset() {
    if (this->lzOut)
    delete this->lzOut;
    if(this->lz)
    delete this->lz;
}
