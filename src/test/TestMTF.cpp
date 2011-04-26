/*
    MTF Unit Test

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
#define BOOST_TEST_MODULE "MTF Testing module"
#include <boost/test/unit_test.hpp>
#include "../codecs/MTF.hpp"
#include "../defines.hpp"
#include <string>
BOOST_AUTO_TEST_SUITE(MTFSuite)



BOOST_AUTO_TEST_CASE(testPaper) {
    std::string* input;
    input = new std::string("testPaper");
    MTF* coder = new MTF(input,NULL,0);
    std::string* output;
    output = new std::string("testPaper.mtf");
    coder->encode(output);
    delete coder;
    delete input;
    delete output;

    input = new std::string("testPaper.mtf");
    coder = new MTF(input,NULL,0);
    output = new std::string("testPaper.umtf");
    coder->decode(output);
    delete coder;
    delete input;
    delete output;
    
    FILE* orig;
    FILE* dec;
    orig = fopen("testPaper","rb");
    dec = fopen("testPaper.umtf","rb");
    
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
