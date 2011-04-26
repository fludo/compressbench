/*
    BWT Unit Test

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
#define BOOST_TEST_MODULE "BWT Testing module"
#include <boost/test/unit_test.hpp>
#include "../codecs/BWT.hpp"
#include "../defines.hpp"
#include <string>
BOOST_AUTO_TEST_SUITE(BWTSuite)



/*BOOST_AUTO_TEST_CASE(testBWT) {
    std::string* input1;
    input1 = new std::string("missi.txt");
    BWTOld* coder1 = new BWTOld(input1,NULL,0);
    std::string* output1;
    output1 = new std::string("missi.txt.bwt");
    coder1->compress(output1);
    delete coder1;
    delete input1;
    delete output1;

    input1 = new std::string("missi.txt.bwt");
    coder1 = new BWTOld(input1,NULL,0);
    output1 = new std::string("missi.txt.ubwt");
    coder1->decompress(output1);
    delete coder1;
    delete input1;
    delete output1;

    FILE* orig1;
    FILE* dec1;
    orig1 = fopen("missi.txt","rb");
    dec1 = fopen("missi.txt.ubwt","rb");

    unsigned char b11,b21;
    while (!feof(orig1)) {
        b11 = fgetc(orig1);
        b21 = fgetc(dec1);
        BOOST_REQUIRE(b11 == b21);
    }
    fclose(orig1);
    fclose(dec1);


}
*/
/*
BOOST_AUTO_TEST_CASE(testBWTPaper) {
    std::string* input2;
    input2 = new std::string("testPaper");
    BWT* coder2 = new BWT(input2,NULL,0);
    std::string* output2;
    output2 = new std::string("testPaper.bwt");
    coder2->compress(output2);
    delete coder2;
    delete input2;
    delete output2;

    input2 = new std::string("testPaper.bwt");
    coder2 = new BWT(input2,NULL,0);
    output2 = new std::string("testPaper.ubwt");
    coder2->decompress(output2);
    delete coder2;
    delete input2;
    delete output2;


    FILE* orig2;
    FILE* dec2;
    orig2 = fopen("testPaper","rb");
    dec2 = fopen("testPaper.ubwt","rb");

    unsigned char b12,b22;
    while (!feof(orig2)) {
        b12 = fgetc(orig2);
        b22 = fgetc(dec2);
        BOOST_REQUIRE(b12 == b22);
    }
    fclose(orig2);
    fclose(dec2);

}

BOOST_AUTO_TEST_CASE(testBWGeo) {
    std::string* input3;
    input3 = new std::string("geo");
    BWT* coder3 = new BWT(input3,NULL,0);
    std::string* output3;
    output3 = new std::string("geo.bwt");
    coder3->compress(output3);
    delete coder3;
    delete input3;
    delete output3;

    input3 = new std::string("geo.bwt");
    coder3 = new BWT(input3,NULL,0);
    output3 = new std::string("geo.ubwt");
    coder3->decompress(output3);
    delete coder3;
    delete input3;
    delete output3;


    FILE* orig3;
    FILE* dec3;
    orig3 = fopen("geo","rb");
    dec3 = fopen("geo.ubwt","rb");

    unsigned char b13,b23;
    while (!feof(orig3)) {
        b13 = fgetc(orig3);
        b23 = fgetc(dec3);
        BOOST_REQUIRE(b13 == b23);
    }
    fclose(orig3);
    fclose(dec3);

}
*/
BOOST_AUTO_TEST_SUITE_END()
