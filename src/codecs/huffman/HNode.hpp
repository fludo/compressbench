/*
    Huffman Coding implementation: Node for tree building
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

#ifndef _HNODE_H
#define	_HNODE_H
#include <vector>
#include <string>
class HNode {
public:
    HNode(unsigned char symbol, double probability);
    HNode(HNode* leftChild, HNode* rightChild);
    HNode(unsigned char symbol, double probability, std::vector<unsigned char> code);
    virtual ~HNode();
    double probability();
    unsigned char symbol();
    bool isTerminal();
    std::vector<HNode*> getTerminals();
    std::vector<unsigned char> code();
    std::string codeString();
    unsigned char numBytes();
private:
    HNode* lchild_;
    HNode* rchild_;
    std::vector<unsigned char> code_;
    unsigned char symbol_;
    double probability_;


        // le constructeur par copie !!! déclaré mais non défini !!!
    HNode(HNode const &);
    // l'opérateur d'affectation !!! déclaré mais non défini !!!
    HNode & operator =(HNode const & );


protected:
    void prependBit(short bitValue);
    
};

#endif	/* _HNODE_H */

