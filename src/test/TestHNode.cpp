/*
    HNode Unit Test

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
#define BOOST_TEST_MODULE "HNode Testing module"
#include <boost/test/unit_test.hpp>
#include "../codecs/huffman/HNode.hpp"
#include <iostream>

BOOST_AUTO_TEST_SUITE(HNodeTest)



BOOST_AUTO_TEST_CASE(testInit) {
    // boost::shared_ptr<HNode> n1
    HNode* n1 = NULL;
    HNode* n2 = NULL;
    HNode* n3 = NULL;
    BOOST_TEST_MESSAGE("Setup HNodes");

    n1 = new HNode('a', 0.004);
    n2 = new HNode('e', 0.29);
    n3 = new HNode(n1, n2);
    BOOST_CHECK(n1->isTerminal() == true);
    BOOST_CHECK(n2->isTerminal() == true);
    BOOST_CHECK(n3->isTerminal() == false);
    BOOST_CHECK(n1->probability() == 0.004);
    std::cerr <<"Probability of n3 is "<< n3->probability()<<std::endl;
    std::cerr <<"Probability of n2 is "<< n2->probability()<<std::endl;
    BOOST_CHECK(n3->probability() == 0.294);
    BOOST_CHECK(n2->probability() == 0.29);
    BOOST_CHECK(n1->symbol() == 'a');
    BOOST_TEST_MESSAGE("Done");
    BOOST_CHECK(n2->symbol() == 'e');
    BOOST_CHECK((n3->getTerminals()).size()==2);
    BOOST_CHECK((n1->getTerminals()).size()==1);
    delete n3;
    BOOST_CHECK(n2->symbol() == 'e');
    BOOST_CHECK(n2->isTerminal() == true);

}

BOOST_AUTO_TEST_SUITE_END()