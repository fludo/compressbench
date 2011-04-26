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
#include "BinaryIO.hpp"


/**
 * allowed mode : 'r'(read) or 'w' (write)
 */
BinaryIO::BinaryIO(const char mode) {
    this->filename = NULL;
    if (mode == 'r') {
        this->write = false;
	
        this->buffer = 0x00;
	this->currBit_ptr = 0;
	//fprintf(stderr,"BinaryIO read mode...\n");
    }
    if (mode == 'w') {
        this->write = true;
	this->buffer = 0x00;
        this->currBit_ptr = 8;
//fprintf(stderr,"BinaryIO write mode...\n");
    }
            


}

BinaryIO::BinaryIO(const char* filename, const char mode) {
    this->filename = new std::string(filename);


    if (this->filename != NULL) {
        if (strcmp(&mode,"r")==0) {
            this->write = false;

            if (!freopen(this->filename->c_str(), "rb", stdin)) {
                //fprintf(stderr,"BinaryIO error opening file...");
            }
            else {
                this->buffer = fgetc(stdin);
                this->currBit_ptr = 8;
            }
        }
        if (strcmp(&mode,"w")==0) {
            this->write = true;
            this->buffer = 0x00;
            this->currBit_ptr = 8;
            if (!freopen(this->filename->c_str(), "wb", stdout)) {
                //fprintf(stderr,"BinaryIO error opening file...");
            }
        }

    }
}

BinaryIO::BinaryIO(const BinaryIO& orig) {
    this->filename = new std::string(*orig.filename);
    this->write = orig.write;
    this->buffer = orig.buffer;
    this->currBit_ptr = orig.currBit_ptr;
}

BinaryIO::~BinaryIO() {
    if (this->currBit_ptr!=8) {
        this->flush();
    }
    if (this->filename) {
        delete this->filename;
        fclose(stdin);
        fclose(stdout);
    }
}

/**
 * Write the current byte, even if uncomplete, replace trailing bits by 0
 */
int BinaryIO::flush() {
    // recall ptr: [87654321], 0 = full
    if (this->write) {
        if (this->currBit_ptr!=0) {
            this->buffer = this->buffer << this->currBit_ptr;
            this->currBit_ptr =0;
        }
        fputc(this->buffer,stdout);
        this->buffer = 0x00;
        this->currBit_ptr = 8;
        return 0;
    }
    else {
        return -1;
    }
}

int BinaryIO::appendBit(unsigned char bit) {
    if (this->currBit_ptr > 0) {
        if (bit==0) {
            this->buffer = this->buffer << 1;
            this->currBit_ptr -=1;
            return 0;
        }
        if (bit==1) {
            this->buffer = this->buffer << 1;
            this->buffer = this->buffer | 0x01;
            this->currBit_ptr -=1;
            return 0;
        }
    }
    else {
        this->flush();
        this->buffer = 0x00;
        this->currBit_ptr = 8;
        this->appendBit(bit);
    }
    return -1;
}

int BinaryIO::appendBits(std::vector<unsigned char> bits) {
    int res = 0;
    //fprintf(stderr,"Appending %u bits",bits.size());
    while (bits.size()!=0) {
        if (this->currBit_ptr == 0) {
            this->flush();
        }
        res = this->appendBit(*(bits.begin()));
        bits.erase(bits.begin());
        if (res!=0) {
            return res;
        }
    }
    return res;
}

unsigned char BinaryIO::readBit() {
    //[87654321]
    if (this->currBit_ptr == 0) {
        if (feof(stdin)) {
            return 0xFF;
        }
        this->buffer = fgetc(stdin);
        this->currBit_ptr = 8;
    }
    unsigned char tmp = (this->buffer >> (this->currBit_ptr-1) ) & 0x01;
    this->currBit_ptr -= 1;
    return tmp;
}

void BinaryIO::readNext(){
  
          this->buffer = fgetc(stdin);
        this->currBit_ptr = 8;
  
}
unsigned char BinaryIO::readByte() {
    if (this->currBit_ptr == 0) {
        unsigned char res;
        res = fgetc(stdin);
        this->buffer = fgetc(stdin);
        this->currBit_ptr = 8;
        return res;
    }

    return this->buffer;
}

bool BinaryIO::eof() {
    return feof(stdin);
}

