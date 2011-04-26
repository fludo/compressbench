/*
    Compression+Coding schema: Huffman(RLE(MTF(BWT(Input)))) (reverse operations for decompression)
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

#include "HuffRleMtfBwt.hpp"

HuffRleMtfBwt::HuffRleMtfBwt(const std::string* filename, void (*debug_ptr)(std::string, int), int verbose_value,bool decompress) {
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
            this->rleOut = new std::string(*filename);
	    this->rleOut->append(".uhrle");
	    this->rle = new RLE(this->huffOut,debug_ptr,verbose_value);
            this->mtfOut = new std::string(*filename);
            this->mtfOut->append(".uhrmtf");
            this->mtf = new MTF(this->rleOut,debug_ptr,verbose_value);

            this->bwt = new BWT(this->mtfOut,debug_ptr,verbose_value);
            this->bwtOut = new std::string(filename->substr(0,filename->rfind(".",filename->size())));
            this->bwtOut->append(".ubmrh");

            fprintf(stderr,"Decompressing...%s -> %s -> %s -> %s -> %s \n",filename->c_str(),this->huffOut->c_str(),this->rleOut->c_str(),this->mtfOut->c_str(), this->bwtOut->c_str());
        }
        else {
            //compressing
            this->bwt = new BWT(filename,debug_ptr,verbose_value);
            this->bwtOut = new std::string(*filename);
            this->bwtOut->append(".bwt");
	    this->mtf = new MTF(bwtOut,debug_ptr,verbose_value);
            this->mtfOut = new std::string(*filename);
            this->mtfOut->append(".bmtf");
            this->rle  = new RLE(mtfOut,debug_ptr,verbose_value);
            this->rleOut = new std::string(*filename);
            this->rleOut->append(".bmrle");
            this->huffman = new Huffman(rleOut,debug_ptr,verbose_value);
            this->huffOut = new std::string(*filename);
            this->huffOut->append(".bmrh");
            fprintf(stderr,"Compressing...%s -> %s -> %s -> %s -> %s\n",filename->c_str(), this->bwtOut->c_str(),this->mtfOut->c_str(),this->rleOut->c_str(),this->huffOut->c_str());
        }

    }
}
void HuffRleMtfBwt::compress(const std::string* outfile) {
    this->bwt->compress(this->bwtOut);
    debug("BWT done...",INFO);
    this->mtf->encode(this->mtfOut);
    debug("MTF done...",INFO);
    this->rle->encode(this->rleOut);
    debug("RLE done...",INFO);
    this->huffman->encode(this->huffOut);
    debug("Huffman done...",INFO);
    debug("Compression complete !",INFO);
}
void HuffRleMtfBwt::decompress(const std::string* output) {
    this->huffman->decode(this->huffOut);
    debug("De-huffman done...",INFO);
    this->rle->decode(this->rleOut);
    debug("De-RLE done...",INFO);
    this->mtf->decode(this->mtfOut);
    debug("De-MTF done...",INFO);
    this->bwt->decompress(this->bwtOut);
    debug("De-BWT done...",INFO);
    debug("Compression complete !",INFO);
}

HuffRleMtfBwt::~HuffRleMtfBwt() {
    delete this->bwtOut;
    delete this->huffOut;
    delete this->rleOut;
    delete this->mtfOut;
    delete this->bwt;
    delete this->rle;
    delete this->mtf;
    delete this->huffman;
}
