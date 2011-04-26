/*
    Huffman.cpp unit Test

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
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Huffman Testing module"
#include <boost/test/unit_test.hpp>
#include "../codecs/Huffman.hpp"
#include "../defines.hpp"
#include <string>
BOOST_AUTO_TEST_SUITE(HuffmanTest)



BOOST_AUTO_TEST_CASE(testCode) {
    std::string* input;
    input = new std::string("TestHuffman.cpp");
    Huffman* coder = new Huffman(input,NULL,0);
    std::string* output;
    output = new std::string("TestHuffman.cpp.huf");
    coder->encode(output);
    delete coder;
    delete input;
    delete output;

}

BOOST_AUTO_TEST_CASE(testDecode) {
    std::string* input;
    input = new std::string("TestHuffman.cpp.huf");
    Huffman* coder = new Huffman(input,NULL,0);
    std::string* output;
    output = new std::string("TestHuffman.cpp.uhuf");
    coder->decode(output);
    delete coder;
    delete input;
    delete output;
   
            FILE* orig;
    FILE* dec;
    orig = fopen("TestHuffman.cpp","rb");
    dec = fopen("TestHuffman.cpp.uhuf","rb");
    
    unsigned char b1,b2;
    while(!feof(orig)){
      b1 = fgetc(orig);
      b2 = fgetc(dec);
      BOOST_CHECK(b1 == b2);
    }
    fclose(orig);
    fclose(dec);


}

BOOST_AUTO_TEST_CASE(testCodePaper) {
    std::string* input;
    input = new std::string("testPaper");
    Huffman* coder = new Huffman(input,NULL,0);
    std::string* output;
    output = new std::string("testPaper.huf");
    coder->encode(output);
    delete coder;
    delete input;
    delete output;

}


BOOST_AUTO_TEST_CASE(testDecodePaper) {
    std::string* input;
    input = new std::string("testPaper.huf");
    Huffman* coder = new Huffman(input,NULL,0);
    std::string* output;
    output = new std::string("testPaper.uhuf");
    coder->decode(output);
    delete coder;
    delete input;
    delete output;
    
    FILE* orig;
    FILE* dec;
    orig = fopen("testPaper","rb");
    dec = fopen("testPaper.uhuf","rb");
    
    unsigned char b1,b2;
    while(!feof(orig)){
      b1 = fgetc(orig);
      b2 = fgetc(dec);
      BOOST_CHECK(b1 == b2);
    }
    fclose(orig);
    fclose(dec);

}

BOOST_AUTO_TEST_CASE(testCodeGeo) {
    std::string* input;
    input = new std::string("geo");
    Huffman* coder = new Huffman(input,NULL,0);
    std::string* output;
    output = new std::string("geo.huf");
    coder->encode(output);
    delete coder;
    delete input;
    delete output;

}

BOOST_AUTO_TEST_CASE(testDecodeGeo) {
    std::string* input;
    input = new std::string("geo.huf");
    Huffman* coder = new Huffman(input,NULL,0);
    std::string* output;
    output = new std::string("geo.uhuf");
    coder->decode(output);
    delete coder;
    delete input;
    delete output;
    
    FILE* orig;
    FILE* dec;
    orig = fopen("geo","rb");
    dec = fopen("geo.uhuf","rb");
    
    unsigned char b1,b2;
    while(!feof(orig)){
      b1 = fgetc(orig);
      b2 = fgetc(dec);
      BOOST_CHECK(b1 == b2);
    }
    fclose(orig);
    fclose(dec);

}
BOOST_AUTO_TEST_SUITE_END()