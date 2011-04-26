/*
    HuffBWT unit Test

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
#define BOOST_TEST_MODULE "HuffBWT Testing module"
#include <boost/test/unit_test.hpp>
#include "../codeschemes/HuffBwt.hpp"
#include "../defines.hpp"
#include <string>
BOOST_AUTO_TEST_SUITE(HuffBWTSuite)

/*

BOOST_AUTO_TEST_CASE(testBWT) {
    std::string* input1;
    input1 = new std::string("missi.txt");
    HuffBwt* coder1 = new HuffBwt(input1,NULL,0,false);
    std::string* output1;
    output1 = new std::string("missi.txt.bh");
    coder1->compress(output1);
    delete coder1;
    delete input1;
    delete output1;

    input1 = new std::string("missi.txt.bh");
    coder1 = new HuffBwt(input1,NULL,0,true);
    output1 = new std::string("missi.txt.ubh");
    coder1->decompress(output1);
    delete coder1;
    delete input1;
    delete output1;

    FILE* orig1;
    FILE* dec1;
    orig1 = fopen("missi.txt","rb");
    dec1 = fopen("missi.txt.uh","rb");

    unsigned char b11,b21;
    while (!feof(orig1)) {
        b11 = fgetc(orig1);
        b21 = fgetc(dec1);
        BOOST_REQUIRE(b11 == b21);
    }
    fclose(orig1);
    fclose(dec1);


}

BOOST_AUTO_TEST_CASE(testHuffBWTPaper) {
    std::string* input2;
    input2 = new std::string("testPaper");
    HuffBwt* coder2 = new HuffBwt(input2,NULL,0,false);
    std::string* output2;
    output2 = new std::string("testPaper.bh");
    coder2->compress(output2);
    delete coder2;
    delete input2;
    delete output2;

    input2 = new std::string("testPaper.bh");
    coder2 = new HuffBwt(input2,NULL,0,true);
    output2 = new std::string("testPaper.ubh");
    coder2->decompress(output2);
    delete coder2;
    delete input2;
    delete output2;


    FILE* orig2;
    FILE* dec2;
    orig2 = fopen("testPaper","rb");
    dec2 = fopen("testPaper.ubh","rb");

    unsigned char b12,b22;
    while (!feof(orig2)) {
        b12 = fgetc(orig2);
        b22 = fgetc(dec2);
        BOOST_REQUIRE(b12 == b22);
    }
    fclose(orig2);
    fclose(dec2);

}
*/
BOOST_AUTO_TEST_CASE(testHuffBWTGeo) {
    std::string* input2;
    input2 = new std::string("geo");
    HuffBwt* coder2 = new HuffBwt(input2,NULL,0,false);
    std::string* output2;
    output2 = new std::string("geo.bh");
    coder2->compress(output2);
    delete coder2;
    delete input2;
    delete output2;

    input2 = new std::string("geo.bh");
    coder2 = new HuffBwt(input2,NULL,0,true);
    output2 = new std::string("geo.ubh");
    coder2->decompress(output2);
    delete coder2;
    delete input2;
    delete output2;


    FILE* orig2;
    FILE* dec2;
    orig2 = fopen("geo","rb");
    dec2 = fopen("geo.ubh","rb");

    unsigned char b12,b22;
    while (!feof(orig2)) {
        b12 = fgetc(orig2);
        b22 = fgetc(dec2);
        BOOST_REQUIRE(b12 == b22);
    }
    fclose(orig2);
    fclose(dec2);

}
BOOST_AUTO_TEST_SUITE_END()
