/*
    LZ78 basic algorithm implementation with varying dictionnary size
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

#include "LZ78.hpp"

namespace LZ{

LZ78::LZ78(const std::string* filename, void (*debug_ptr)(std::string, int) = NULL, int verbose_value = 0) {
    this->dict = NULL;
    this->decode_dict = NULL;
    this->verbose = verbose_value;
    if (debug_ptr != NULL) {
        this->debug = debug_ptr;
    } else {
        this->debug = null_debug;
    }

    if (filename != NULL) {
        debug("Running LZ78 constructor, file is : " + *filename, DEBUG);
        this->filename = new string(*filename);
        //debug(this->filename,DEBUG);
    } else {
        this->filename = NULL;
    }
}

void LZ78::compress(const std::string* outfile) {
    //unsigned char last_index = 0x00;
    unsigned long last_index = 0x00000000;
    this->dict = new map<std::vector<unsigned char>, unsigned long>();
    //char buffer[1];
    unsigned char buffer;
    //size_t size = this->filename->size() + 1;
    //char * bufferName = new char[ size ];
    // copier la chaîne
    debug("Compressing...", DEBUG);
    if (this->filename != NULL) {
        debug("Opening from file : " + *(this->filename), DEBUG);
        if (!freopen(this->filename->c_str(), "rb", stdin)) {
            debug("Error opening file !", CRITICAL);
        }
    }
    //strncpy(bufferName, this->filename.c_str(), size);

    //ifstream file(bufferName, std::ios::in | std::ios::binary);
    //delete bufferName;

    //FILE *fp;
    if (outfile != NULL) {
        debug("Opening outfile : " + *outfile, DEBUG);
        if (!freopen(outfile->c_str(), "wb", stdout)) {
            debug("Error opening file for output !", CRITICAL);
        }
    }

    // Since EOF can be returned by fgect, feof(STREAM*) has to be used
    // for binary files
    bool eofile = false;
    std::vector<unsigned char> currBlock;

    while ((!eofile)) {
        // read a new Byte
        buffer = fgetc(stdin);

        currBlock.push_back(buffer);

        if (feof(stdin)) {
            debug("End Of file", DEBUG);
            eofile = true;
            //read to far :-)
            currBlock.pop_back();
            continue;
        }
        if (ferror(stdin)) {
            // in case of error, terminate compression loop
            debug("ERROR while reading file, please submit a bug report to <ludovic.favre@epfl.ch> including your file and the command you entered", CRITICAL);
            break;
        } else {
            // if everything is ok, compression can proceed

            if (this->dict->size() >= MAX_DICSIZE) {
                debug("Cleaning dictionary, it was full !",DEBUG);
                this->dict->clear();
                last_index = 0x00000000;
            }

            if (isInDictionary(currBlock) != 0 && this->dict->size() < MAX_DICSIZE) {
                debug("Found  currBlock  in dictionnary, continuing match !", DEBUG);
                // in dictionary, try to find the longest match
            } else {
                debug("Not in dictionary !", DEBUG);
                // not in dictionary
                //length 1, this is the start... just add it in dict
                //length >1, then previous string was matched
                last_index += 0x00000001;


                //fprintf(stderr,"last index is %lu \n",last_index);

                if (currBlock.size() == 1) {
		    
                    this->dict->insert(std::pair<std::vector<unsigned char>, unsigned long>(currBlock, last_index));

                    //debug(ulongToString(&last_index) + "[" + buffer + "]", DEBUG);

                    writeDictToByte(MISS);
                    fputc(currBlock.front(), stdout);
                    fflush(stdout);
                } else {
                    //fprintf(stderr,"Adding to dict...\n");
                    // substring was a "match"
                    std::vector<unsigned char> prev = std::vector<unsigned char>(currBlock);
                    prev.pop_back();
                    unsigned long match = isInDictionary(prev);
                    this->dict->insert(std::pair<std::vector<unsigned char>, unsigned long>(currBlock, last_index));

                    // debug("Block of symbol:" + currBlock + "(" + makestring(last_index) + ")", DEBUG);
                    //debug(ulongToString(&last_index) + "[" + buffer + "]", DEBUG);

                    writeDictToByte(SET_MATCH(match));
                    //fputc(currBlock.substr(currBlock.size() - 1, currBlock.size()).c_str()[0], stdout);
                    fputc(currBlock.at(currBlock.size()-1),stdout);
                    fflush(stdout);
                }
                currBlock.clear();
            }
        }

    }
    
    // EOFILE reached
    if (currBlock.size() != 0) {
        // was found in dictionary but has not been inserted in output file, this is the last word
        //debug(makestring(isInDictionary(currBlock)), DEBUG);
        /*for (std::vector<unsigned char>::iterator it = currBlock.begin(); it!=currBlock.end(); ++it) {
            fprintf(stderr,"0x%x ",*it);
        }*/
        if (isInDictionary(currBlock)) {
            if (currBlock.size()!=1) {
                //fprintf(stderr,"Is dict ! :D %lu",isInDictionary(currBlock));
                unsigned char trailing;
                trailing = *(currBlock.end()-1);
                currBlock.pop_back();
                writeDictToByte(SET_MATCH(isInDictionary(currBlock)));
                fputc(trailing,stdout);
            } else {
		//fprintf(stderr,"Not in dict :(\n");
                writeDictToByte(MISS);
		fputc(currBlock.at(0),stdout);
            }
        }
        else {
            //end of block prevends from getting dictionary entry...
            //fprintf(stderr,"Not in dict :(");
            std::vector<unsigned char> copy(currBlock);
            copy.pop_back();
            writeDictToByte(SET_MATCH(isInDictionary(copy)));
	    writeDictToByte(MISS);
            //fputc(MISS,stdout);
            fputc(*(currBlock.end()-1),stdout);
        }
    }

    /*
     * Manual termination of the compressed file (depends on DICT_NUMBYTE)
     */
    if (DICT_NUMBYTE == 3) {
        fputc(0x7F, stdout);
    }
    if (DICT_NUMBYTE >= 2) {
        if (DICT_NUMBYTE == 2) {
            fputc(0x7F, stdout);
        } else {
            fputc(0xFF, stdout);
        }
    }
    if (DICT_NUMBYTE >= 1) {
        if (DICT_NUMBYTE == 1) {
            fputc(0x7F, stdout);
        } else {
            fputc(0xFF, stdout);
        }
    }
    fclose(stdin);
    fclose(stdout);
    debug("Done !", DEBUG);
}

void LZ78::decompress(const std::string* outfile) {
    unsigned long last_index = 0x00000000;
    decode_dict = new std::map<unsigned long, std::vector<unsigned char> > ();
    //fprintf(stderr, "Max map size is %lu\n", this->decode_dict->max_size());
    unsigned long maskIndex, eofile;
    eofile = EOFILE;
    unsigned char newEntry;

    debug("Decompressing...", INFO);
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


    bool terminated = false;
    int byteIndex;
    unsigned char currByte, lookahead1, lookahead2;
    long lastPos;
    std::vector<unsigned char> part;
    do {

        maskIndex = 0;
        for (byteIndex = 0; byteIndex < DICT_NUMBYTE - 1; byteIndex++) {
            currByte = fgetc(stdin);
            maskIndex = maskIndex + currByte;
            maskIndex = maskIndex << 8;
        }
        currByte = fgetc(stdin);
        maskIndex = maskIndex + currByte;
        //fprintf(stderr, "Maskindex = %lu\n ******************************\n", maskIndex);
        //cerr << "\t Index is:" << std::bitset < 24 > (maskIndex) << std::endl;
        if (IS_MATCH(maskIndex)) {
            debug("MATCH !", DEBUG);
        } else {
            debug("MISS !", DEBUG);
        }
        if (maskIndex == eofile) {
            debug("end of file", DEBUG);
            terminated = true;
        } else {
            //fprintf(stderr,"index : %lu\n",CLEAN_FLAG(maskIndex));
            newEntry = fgetc(stdin);
            //fprintf(stderr,"Entry = %x\n",newEntry);
            lastPos = ftell(stdin);
            bool eofMark = false;
            if (DICT_NUMBYTE>1) {
                lookahead1 = fgetc(stdin);
                if (newEntry == 0x7F && lookahead1 == 0xFF)
                    eofMark = true;
            }
            if (DICT_NUMBYTE>2) {
                lookahead2 = fgetc(stdin);
                if (eofMark && lookahead2 == 0xFF)
                    eofMark = true;
		else
		    eofMark = false;
            }
	    

            if (!eofMark) {


                fseek(stdin, 1-DICT_NUMBYTE, SEEK_CUR);

                //fprintf(stderr, "last index is %lu \n", last_index);
                last_index += 0x00000001;
                if (IS_MATCH(maskIndex)) {
                    debug("Decoding a match in dict", DDEBUG);
                    // match -> 1??????? ........ where ?? is dict index, ... is symbol coding
                    unsigned long index = CLEAN_FLAG(maskIndex);
                    //cerr << "\t Real Index is:" << std::bitset < 24 > (index) << std::endl;
                    part.clear();
                    part = getEntry(index);
                    if (part.size() ==0) {
                        //fprintf(stderr,"\nsize0!\n");
                        debug("Entry not found in dictionary, internal error !",CRITICAL);
			return;
                        // break;
                    }

                    debug("Entry has been found", DEBUG);
                    part.push_back(newEntry);
		    //fprintf(stderr,"Pushing new entry in dictionary : %x\n",newEntry);
                    debug("String appended", DDEBUG);

                    this->decode_dict->insert(std::pair<unsigned long, std::vector<unsigned char> > (last_index, std::vector<unsigned char>(part)));
                    if (this->decode_dict->size() >=MAX_DICSIZE) {
                        this->decode_dict->clear();
                        last_index = 0x00000000;
                    }
                    for (std::vector<unsigned char>::iterator it = part.begin(); it!=part.end(); ++it) {
                        fputc(*it, stdout);
                    }
                    part.clear();


                }
                else {
                    debug("No match for current DWORD Adding to dictionary", DDEBUG);
                    std::vector<unsigned char> vect;
                    vect.push_back(newEntry);
                    this->decode_dict->insert(std::pair<unsigned long, std::vector<unsigned char> > (last_index, vect));
		                        if (this->decode_dict->size() >=MAX_DICSIZE) {
                        this->decode_dict->clear();
                        last_index = 0x00000000;
                    }
                    fputc(newEntry, stdout);

                }
            } else {
                debug("Special EOF situation !", DEBUG);
		//fprintf(stderr,"lookahead1 = %x \tlookahead2 = %x \t newEntry = %x\n",lookahead1,lookahead2,newEntry);

                if (IS_MATCH(maskIndex)) {
                    debug("Decoding a match in dict", DDEBUG);
                    unsigned long index = CLEAN_FLAG(maskIndex);
                    part.clear();
                    part = getEntry(index);
                    if (part.size() ==0) {
                        //fprintf(stderr,"\nsize0!\n");
			//fprintf(stderr,"Cannot find entry %lu\n",index);
                        debug("Entry not found in dictionary, internal error !",CRITICAL);
			return;
                    }
                    else
                    {
                    debug("Entry has been found", DEBUG);
		    //fprintf(stderr,"Found entry %lu\n",index);
                    for (std::vector<unsigned char>::iterator it = part.begin(); it!=part.end(); ++it) {
                        //fprintf(stderr,"->0x%x",*it);
                        fputc(*it, stdout);
                    }
		    }
                }
                part.clear();
                terminated = true;
            }


        }

    } while (maskIndex != eofile && !terminated);
    if (part.size()!=0) {
        debug("hm, Part is not empty !",CRITICAL);
    }
    debug("Closing stdin FILE descriptor", DDEBUG);
    fclose(stdin);
    debug("Closing stdout FILE descriptor", DDEBUG);
    fclose(stdout);
    debug("Done !", INFO);
}

unsigned long LZ78::isInDictionary(std::vector<unsigned char> entry) {
    std::map<std::vector<unsigned char>, unsigned long>::iterator it;
    it = this->dict->find(entry);

    debug("#################", DDEBUG);
    /*
    if (verbose >= DDEBUG) {
        for (std::map<std::string, unsigned long>::const_iterator it1 = this->dict->begin(); it1 != this->dict->end(); ++it1) {
            debug("String(key = first): " + it1->first + " |  Int(value = second): " + makestring(it1->second), DDEBUG);
        }
    }
     */


    if (it != this->dict->end()) {
        //debug("Value is: " + it->second, DDEBUG);
        return it->second;
    } else {
        //debug("Substring ->" + entry + "<- not found in dictionary", DDEBUG);
        return 0;
    }
    debug("#################", DDEBUG);
}

std::vector<unsigned char> LZ78::getEntry(unsigned long index) {
    std::map<unsigned long, std::vector<unsigned char> >::iterator it;
    it = this->decode_dict->find(index);
    //fprintf(stderr, "getEntry() looking for : %lu", index);
    if (it != this->decode_dict->end()) {
        debug("Decoded text!", DDEBUG);
        //fprintf(stderr,"Decoded entry of size : %d\n",it->second.size());
        return std::vector<unsigned char>(it->second);
    } else {
        debug("Entry not found in dictionary", DDEBUG);
        return std::vector<unsigned char>();
    }

}

void LZ78::writeDictToByte(unsigned long value) {
    // transform an unsigned long of the form 0x00000000 and
    // write it for DICT_NUMBYTE bytes to stdout
    unsigned long tmp = value;
    unsigned char toprint;
    //fprintf(stderr, "Writing index=> %lu <=\n", tmp);
    if (DICT_NUMBYTE == 3) {
        // 0x00[00][00][00] and first [] block sould be written first !
        //fprintf(stderr, "Writing 0x00[FF]0000=> %s <=\n", &toprint);
        toprint = (tmp & 0x00FF0000) >> 16;
        fputc(toprint, stdout);

    }
    if (DICT_NUMBYTE >= 2) {
        //fprintf(stderr, "Writing index2=> %lu <=\n", tmp);
        // 0x0000[00][00] and first [] block sould be written first !
        toprint = (tmp & 0x0000FF00) >> 8;
        //fprintf(stderr, "Writing 0x0000[FF]00=> %s <=\n", &toprint);
        fputc(toprint, stdout);
    }
    if (DICT_NUMBYTE >= 1) {
        //fprintf(stderr, "Writing index1=> %lu <=\n", tmp);
        // 0x000000[00] default to 1 byte for the dictionary
        toprint = (tmp & 0x000000FF);
        //fprintf(stderr, "Writing 0x000000[FF]=> %s <=\n", &toprint);
        fputc(toprint, stdout);
    }
}

LZ78::~LZ78() {
    if (this->dict != NULL) {
        delete this->dict;
        this->dict = NULL;
    }
    if (this->decode_dict != NULL) {
        delete this->decode_dict;
        this->decode_dict = NULL;
    }

    if (this->filename != NULL) {
        delete this->filename;
        this->filename = NULL;
    }
}

}