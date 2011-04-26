/*
    Run-length encoding implementation
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

#include "RLE.hpp"

RLE::RLE(const std::string* infilename, void (*debug_ptr)(std::string, int) = NULL, int verbose_value = 0) {
    this->verbose = verbose_value;
    if (debug_ptr != NULL) {
        this->debug = debug_ptr;
    } else {
        this->debug = null_debug;
    }

    if (infilename != NULL) {
        debug("Running RLE constructor, file is : " + *infilename, DEBUG);
        this->filename = new string(*infilename);
        //debug(this->filename,DEBUG);
    } else {
        this->filename = NULL;
    }
}

RLE::RLE(const RLE& orig) {
    this->verbose = orig.verbose;
    this->debug = orig.debug;
    this->filename = orig.filename;
}

RLE::~RLE() {
    if(this->filename != NULL){
            delete this->filename;
            this->filename = NULL;
    }

}

void RLE::encode(const std::string*outfile) {

    if (this->filename != NULL) {
        debug("Opening from file : " + *(this->filename), DEBUG);
        if (!freopen(this->filename->c_str(), "rb", stdin)) {
            debug("Error opening file !", CRITICAL);
        }
    }

    if (outfile != NULL) {
        debug("Opening outfile : " + *outfile, DEBUG);
        if (!freopen(outfile->c_str(), "wb", stdout)) {
            debug("Error opening file for output !", CRITICAL);
        }
    }
    bool eofile = false;;
    unsigned char buffer;
    unsigned char count;



    while ((!eofile)) {
        // read a new Byte
        buffer = fgetc(stdin);
        count = 0x01;
        debug("----------------", DEBUG);

        if (feof(stdin)) {
            debug("End Of file", DEBUG);
            eofile = true;
            continue;
        }

        while(!feof(stdin) && buffer == fgetc(stdin) && count<0xFF){
            count+=0x01;
        }
                if (feof(stdin)) {
            debug("End Of file", DEBUG);
            eofile = true;
        }
        //we are one position too far, rewind
        fseek(stdin, -1, SEEK_CUR);
        // write the pair of byte [symbol][count]
        fputc(buffer,stdout);
        fputc(count,stdout);

    }

    debug("RLE encoding done", DEBUG);
    fclose(stdout);
}

void RLE::decode(const std::string*outfile) {
    if (this->filename != NULL) {
        debug("Opening from file : " + *(this->filename), DEBUG);
        if (!freopen(this->filename->c_str(), "rb", stdin)) {
            debug("Error opening file !", CRITICAL);
        }
    }

    if (outfile != NULL) {
        debug("Opening outfile : " + *outfile, DEBUG);
        if (!freopen(outfile->c_str(), "wb", stdout)) {
            debug("Error opening file for output !", CRITICAL);
        }
    }

    bool eofile = false;
    unsigned char symbol;
    unsigned char count;

    while ((!eofile)) {
// read a new Byte
        symbol = fgetc(stdin);
        count = fgetc(stdin);
        debug("----------------", DEBUG);

        if (feof(stdin)) {
            debug("End Of file", DEBUG);
            eofile = true;
            continue;
        }

        // we are using unsigned char... dont use signed comparison
        while(count!=0x00){
            fputc(symbol,stdout);
            count-=0x01;
        }
    }

    debug("RLE decoding done", DEBUG);
    fclose(stdout);
}