/*
    Move-To-Front implementation
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

#include "MTF.hpp"

MTF::MTF(const std::string* infilename, void (*debug_ptr)(std::string, int) = NULL, int verbose_value = 0) {
    this->verbose = verbose_value;
    if (debug_ptr != NULL) {
        this->debug = debug_ptr;
    } else {
        this->debug = null_debug;
    }

    if (infilename != NULL) {
        debug("Running MTF constructor, file is : " + *infilename, DEBUG);
        this->filename = new string(*infilename);
        //debug(this->filename,DEBUG);
    } else {
        this->filename = NULL;
    }
}

MTF::MTF(const MTF& orig) {
  this->verbose = orig.verbose;
  this->debug = orig.debug;
}

MTF::~MTF() {
}

void MTF::encode(const std::string*outfile) {

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

    // working with bytes, array size of 256 is required
    unsigned char mtfList[256];
    // initialize with bytes value from 0 to 256 (would be the same for decode part)
    for (int i = 0; i < 256; i++) {
        mtfList[i] = (unsigned char) i;
    }
    bool eofile = false;
    unsigned char buffer;
    unsigned char index;

    while ((!eofile)) {
        // read a new Byte
        buffer = fgetc(stdin);

        //debug("----------------", DEBUG);

        if (feof(stdin)) {
            debug("End Of file", DEBUG);
            eofile = true;
            continue;
        }

        /*
         * Linear search...
         * no better choice since it is not ordered
         */
        for (index = 0; ; index++) {
            if (mtfList[index] == (buffer & 0xFF))
                break;
	    if(index == 0xFF){
	      debug("Error search symbol for MTF",CRITICAL);
	      break;
	    }
        }

        // write index
        fputc(index, stdout);

        /* Move the element to the head of the list after shifting preceeding
         * elements
         */
        for (int i = index; i > 0; i--) {
            mtfList[i] = mtfList[i - 1];
        }
        mtfList[0] = (unsigned char) buffer;
    }

    debug("MTF encoding done", DEBUG);
    fclose(stdout);
    fclose(stdin);
}

void MTF::decode(const std::string*outfile) {
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

    // working with bytes, array size of 256 is required
    unsigned char mtfList[256];
    // initialize with bytes value from 0 to 256 (would be the same for decode part)
    for (int i = 0; i < 256; i++) {
        mtfList[i] = (unsigned char) i;
    }
    bool eofile = false;
    unsigned char buffer;
    unsigned char index;

    while ((!eofile)) {
        // read a new Byte
        index = fgetc(stdin);

        debug("----------------", DEBUG);

        if (feof(stdin)) {
            debug("End Of file", DEBUG);
            eofile = true;
            continue;
        }
        /* output the "decoded" byte */
        fputc(mtfList[index], stdout);
        buffer = mtfList[index];


        /* Symbol has to be moved to the top */
        for (int i = index; i > 0; i--) {
            mtfList[i] = mtfList[i-1];
        }

        mtfList[0] = buffer;
    }

    debug("MTF decoding done", DEBUG);
    fclose(stdin);
    fclose(stdout);
}

