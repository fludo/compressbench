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


#include "Huffman.hpp"

Huffman::Huffman(const std::string* infilename, void (*debug_ptr)(std::string, int) = NULL, int verbose_value = 0) {
    this->verbose = verbose_value;
    if (debug_ptr != NULL) {
        this->debug = debug_ptr;
    } else {
        this->debug = null_debug;
    }
    /*
    if (sizeof(long)!=4) {
        debug("Invalid long size on this platform !",CRITICAL);
    }*/
    if (infilename != NULL) {
        debug("Running RLE constructor, file is : " + *infilename, DEBUG);
        this->filename = new string(*infilename);
        //debug(this->filename,DEBUG);
    } else {
        this->filename = NULL;
    }
}

Huffman::Huffman(const Huffman& orig) {
    this->verbose = orig.verbose;
    this->debug = orig.debug;
    this->filename = new std::string(*(orig.filename));
}

Huffman::~Huffman() {
    delete this->filename;
    this->filename = NULL;
}

void Huffman::encode(const std::string* outfile) {

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
     * Phase 1: build an ordered (by probability) list of
     * symbol + probability (requires full file parsing)
     *
     */
    unsigned char buffer;
    bool eofile = false;
    //initialize symbol count
    unsigned long count[256];
    unsigned long total = 0;


    std::set<HNode*,HNodeSorting> activeNodes;

    for (int i = 0; i < 256; i++) {
        count[i] = 0;
    }

    debug("READING FILE",DEBUG);

    while ((!eofile)) {
        // read a new Byte
        buffer = fgetc(stdin);

        //debug("----------------", DEBUG);

        if (feof(stdin)) {
            debug("End Of file", DEBUG);
            eofile = true;
            continue;
        }

        count[buffer] += 1;
        if (ULONG_MAX<=count[buffer]) {
            debug("Overflow in the symbol counter, file cannot exceed 4GB !",CRITICAL);
        }
        total += 1;
    }

    debug("Input file read",DDEBUG);
    //rewind stream for encoding phase
    rewind(stdin);
    debug("Rewinded",DDEBUG);
    double sum1=0.0;
    for (int i = 0; i < 256; i++) {

        double probability = (double)(((double)count[i]) / ((double)total));
        if (count[i] !=0) {
            //fprintf(stderr,"Node %d created, new size is %d\n",i,activeNodes.size());
            sum1+=probability;
            activeNodes.insert(new HNode(i, probability));
        }
    }

    assert(sum1-1.0<=1e-10 && sum1>0.0);
    //fprintf(stderr,"Nodes created %f, total node=%d\n",sum1,activeNodes.size());



    /*
     * Phase 2: iteratively merge the two nodes of lower probability until
     * only one node is remaining. Everytime a merging node is created, transmit
     * the code to children.
     *
     */
    HNode* first;
    HNode* second;
    while (activeNodes.size()!=1) {
        first = *(activeNodes.begin());
        activeNodes.erase(activeNodes.begin());
        second = *(activeNodes.begin());
        activeNodes.erase(activeNodes.begin());
        activeNodes.insert(new HNode(first,second));
        //fprintf(stderr,"Merging... (%d remaining)\n",activeNodes.size());
    }

    /*
     * Phase 3: recover leaf nodes, deleting others non-leaf nodes. Those leaf
     * nodes contains the code for a given symbol and will be our encoding
     * dictionary
     */

    HNode* root = *(activeNodes.begin());
    std::vector<HNode*> terminals = root->getTerminals();
    //fprintf(stderr,"Terminals: %d\n",terminals.size());

    //debug only...
    /*
    for(int i = 0;i<terminals.size();i++){
      HNode* currentEntry = terminals.at(i);
      fprintf(stderr,"Node <%u>: %f  size+code (%d)>%s<\n",currentEntry->symbol(),currentEntry->probability(),currentEntry->code().size(),
          currentEntry->codeString().c_str());
    }*/

    /*
     * Phase 4: write the encoding table to the file header (see spec)
     * Then simply do binary encoding of the input file according to coding table
     */
    std::map<unsigned char,std::vector<unsigned char> > table;
    //ENCODING TABLE TO HEADER
    BinaryIO* binIO = new BinaryIO('w');
    for (unsigned int i = 0;i<terminals.size();i++) {
        HNode* currentEntry = terminals.at(i);
        //fprintf(stderr,"Node <%x>: %f  size+code (%d-->%d)>%s<\n",currentEntry->symbol(),currentEntry->probability(),currentEntry->numBytes(),
        //        currentEntry->code().size(),currentEntry->codeString().c_str());
        fputc(currentEntry->symbol(),stdout);
        fputc(currentEntry->code().size(),stdout);
        binIO->appendBits(currentEntry->code());
        binIO->flush();
        table.insert(std::pair<unsigned char,std::vector<unsigned char> >(currentEntry->symbol(),currentEntry->code()));
    }
    //fprintf(stderr,"Wrote %d decode-entry\n",table.size());
    fputc(0x00,stdout);
    fputc(0x00,stdout);
    //fprintf(stderr,"Closed..\n");
    debug("Header written !",DDEBUG);
    //ENCODING FILE
    //fprintf(stderr,"To read: %lu\n",total);
    //number of byte coded
    fputc((total>>24)&0xFF,stdout);
    fputc((total>>16)&0xFF,stdout);
    fputc((total>>8)&0xFF,stdout);
    fputc(total&0xFF,stdout);
    std::map<unsigned char,std::vector<unsigned char> >::iterator it1;

    eofile = false;
    while ((!eofile)) {
        // read a new Byte
        buffer = fgetc(stdin);

        if (feof(stdin)) {
            debug("End Of file", DEBUG);
            eofile = true;
            continue;
        }

        it1 = table.find(buffer);

        if (it1 != table.end()) {
            //debug("Value is: " + it->second, DDEBUG);
            //fprintf(stderr,"%d",(*it1).second.size());
            binIO->appendBits((*it1).second);
        } else {
            debug("Huffman: Cannot find a symbol in the coding table !!!",CRITICAL);
        }
    }

    binIO->flush();
    //CLEANUP
    delete binIO;

    // Check the validity of the code, this can be removed to improve performances !
    double sum = 0.0;
    //fprintf(stderr,"Cleaning %d\n",activeNodes.size());
    set<HNode*>::iterator it;


    for (it = activeNodes.begin(); it != activeNodes.end(); it++) {

        HNode* currentEntry = *it;
        sum+=currentEntry->probability();
        //fprintf(stderr,"Node <%x>: %f\n",currentEntry->symbol(),currentEntry->probability());
        delete currentEntry;
    }
    assert(sum-1.0<=1e-15 && sum>0.0);
    debug("Huffman coding done",DEBUG);
    //fprintf(stderr,"Done %f",sum);
    fclose(stdout);
    fclose(stdin);

}

void Huffman::decode(const std::string* outfile) {
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
    * Phase 1: read the encoding table from the file header (see spec)
    */
    unsigned char symbol;
    unsigned char numbits;
    std::vector<unsigned char> code;
    bool eofile = false;
    unsigned long toread=0;
    std::map<std::vector<unsigned char>,unsigned char> table;


    //fprintf(stderr,"READING FILE\n");
    BinaryIO binIO('r');
    while ((!eofile)) {
        code.clear();
        //[symbol][used bits][code ... ] (code truncated to superior byte value, [] = byte)
        symbol = fgetc(stdin);
        numbits = fgetc(stdin);
        if (feof(stdin)|| (symbol == 0x00 && numbits == 0x00)) {
            debug("End Of header", DEBUG);
            eofile = true;
            if (!feof(stdin)) {
                toread += fgetc(stdin)&0xFF;
                toread = toread << 8;
                toread += fgetc(stdin)&0xFF;
                toread = toread << 8;
                toread += fgetc(stdin)&0xFF;
                toread = toread << 8;
                toread += fgetc(stdin)&0xFF;
                binIO.readNext();
            }
            continue;
        }
        binIO.readNext();
        for (int i=0;i<numbits;i++) {

            unsigned char bit = binIO.readBit();
            if (bit == 0xFF)
                debug("Error while decoding bit...",CRITICAL);
            else
                code.push_back(bit);
        }
        //binIO.readNext();
        //fprintf(stderr,"new code for %x of size :%d=%d\n",symbol,numbits,code.size());
        table.insert(std::pair<std::vector<unsigned char>,unsigned char>(code,symbol));

    }
    // do not rewing for decoding..
    eofile = false;
    // fprintf(stderr,"Got a table of size : %d\n",table.size());

    /*
    * Phase 2: after skipping the End-Of-Header Market 0x00 0x00
    * start decoding the file
    */
    code.clear();
    std::map<std::vector<unsigned char>,unsigned char>::iterator it;


    unsigned long writtenBytes = 0;
    //fprintf(stderr,"To read (decode): %lu\n",toread);
    while (!binIO.eof()&& writtenBytes!=toread) {
        unsigned char bit = binIO.readBit();
        //fprintf(stderr,"%x",bit);
        code.push_back(bit);
        it = table.find(code);

        if (it != table.end()) {
            //fprintf(stderr," ");
            fputc((*it).second,stdout);
            writtenBytes+=0x01;
            code.clear();
        } else {
            /*fprintf(stderr,"Miss(%d)",code.size());
                   debug("NoMatch at this stage...",DDEBUG);*/
        }
    }
    fclose(stdout);
    fclose(stdin);
}
