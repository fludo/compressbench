/*
    Burrows-Wheeler Transform class
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

#include "BWT.hpp"
using namespace std;

BWT::BWT(const std::string* infilename, void (*debug_ptr)(std::string, int) = NULL, int verbose_value = 0) {
    this->verbose = verbose_value;
    if (debug_ptr != NULL) {
        this->debug = debug_ptr;
    } else {
        this->debug = null_debug;
    }

    if (infilename != NULL) {
        debug("Running BWT constructor, file is : " + *infilename, DEBUG);
        this->filename = new string(*infilename);
        //debug(this->filename,DEBUG);
    } else {
        this->filename = NULL;
    }
}

/*BWT::BWT(const BWT& orig) {
}*/

void BWT::compress(const std::string* outfile) {



    //BWT compression use a pipeline like: BWT -> MTF -> RLE -> HUFFMAN/ARITH
    // copier la chaîne
    debug("BWT in progress...", INFO);

    // if file already exists, dont wast time on re-performing BWT.. but notify the user...


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
    /*
     * Input: [ToBWT        ]
     * Output:[BWT'ed block ][OrigPos] (2nd block will take the required number of byte... ie log2(size(bwted block))
     */
    unsigned long written;
    /* Block-processing loop
     */
    for ( ; ; ) {
//
// After reading in the data into the buffer, I do some
// UI stuff, then write the length out to the output
// stream.
//
        curr_length = fread( buffer, 1, BLOCKSIZE, stdin );
        if ( curr_length == 0 )
            break;

        memcpy(doubleBuffer,buffer,curr_length);
        memcpy(doubleBuffer+curr_length,buffer,curr_length);

        unsigned long i;
        set< unsigned char *, RuntimeCmp > p;
        for ( i = 0 ; i <= curr_length ; i++ ) {
            p.insert( buffer + i );
        }
        set< unsigned char *, RuntimeCmp >::iterator ii;

        set< unsigned char *, RuntimeCmp >::iterator tmp;
        tmp  = p.end();
        tmp --;
        long trueBlockPos;
        //find position in the permutation
        for ( i = 0, ii = p.begin() ; ii != p.end() ; i++, ii++ ) {
            if ( *ii == buffer ) {
                trueBlockPos = i-1;

            } else
                fputc( (*ii)[ -1 ], stdout );
        }
        p.erase( p.begin(), p.end() );
        p.clear();

        written = fwrite( &trueBlockPos, 1, sizeof( unsigned long ), stdout );
    }



//fprintf(stderr," \n ******** End BWT************ \n");
    fclose(stdin);
    fclose(stdout);
}

void BWT::decompress(const std::string* output) {
    //BWT compression use a pipeline like: BWT -> MTF -> RLE -> HUFFMAN/ARITH
    // copier la chaîne
    debug("De-BWT...", INFO);
    //fprintf(stderr,"block of size %lu\n",BLOCKSIZE);
    if (this->filename != NULL) {
        debug("Opening from file : " + *(this->filename), DEBUG);
        if (!freopen(this->filename->c_str(), "rb", stdin)) {
            debug("Error opening file !", CRITICAL);
        }
    }

    if (output != NULL) {
        debug("Opening outfile : " + *output, DEBUG);
        if (!freopen(output->c_str(), "wb", stdout)) {
            debug("Error opening file for output !", CRITICAL);
        }
    }
    //fprintf(stderr," \n ******** De-BWT ************ \n");

    bool loop = true;
    unsigned long count[ 257 ];
    unsigned long trVect[ BLOCKSIZE];
    unsigned long origPos;
    unsigned long i;
    unsigned long readSize;
    while (loop) {
        //read a block, report the size read in curr_length
        curr_length = fread(buffer, 1, BLOCKSIZE, stdin);
        if (curr_length > sizeof(unsigned long) && curr_length!=BLOCKSIZE) {
            curr_length  -= sizeof(unsigned long);
            fseek(stdin,-sizeof(unsigned long),SEEK_CUR);
        }

        if (curr_length <= 0) {
            loop = false;
            debug("Read block of size 0, terminating",DEBUG);
            break;
        }




        readSize = fread( &origPos, 1, sizeof( unsigned long ), stdin);
        if (readSize< sizeof(unsigned long)) {
            unsigned long diff = sizeof(unsigned long) - readSize;
            curr_length-= diff;
            fseek(stdin,-sizeof(unsigned long),SEEK_CUR);
            readSize =fread( &origPos, 1, sizeof( unsigned long ), stdin);
        }
//        std::cerr << "Read Block of size : " << curr_length << std::endl;
//        fprintf(stderr,"Processing block of size %lu with orig pos = %lu \n",this->curr_length,origPos);



        // initialize count to create vector as in the report
        for ( i = 0 ; i < 256 ; i++ ) {
            count[ i ] = 0;
        }
        for ( i = 0 ; i <(unsigned long) curr_length ; i++ ) {
            count[ buffer[ i ] ]++;
        }

        //accumulate
        for ( i = 1 ; i < 256 ; i++ ) {
            count[ i ] += count[i-1];
        }

        //and shift
        for ( i = 255 ; i > 0 ; i-- ) {
            count[ i ] = count[i-1];
        }
        count[0] = 0;

        for ( i = 0 ; i < curr_length ; i++ ) {
            trVect[count[buffer[i]]++] = i;
        }


        unsigned long j;
        i = origPos;
        for ( j = 0 ; j < ( curr_length ) ; j++ ) {
            i = trVect[ i ];
            putc( buffer[ i ], stdout );



        }
        fflush(stdout);




    }
    fclose(stdin);
    fclose(stdout);
}

BWT::~BWT() {
    if (this->filename !=NULL) {
        delete this->filename;
    }
}

inline unsigned int BWT::iLog2(unsigned int value)
{
    unsigned int l = 0;
    while ( (value >> l) > 1 ) ++l;
    return l;
}

