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

#include <time.h>

#include "HNode.hpp"
#include "stdio.h"
#include <bits/stringfwd.h>

HNode::HNode(unsigned char symbol, double probability) {
    this->lchild_ = NULL;
    this->rchild_ = NULL;
    this->symbol_ = symbol;
    this->probability_ = probability;
    this->code_ = std::vector<unsigned char>();
}

HNode::HNode(unsigned char symbol, double probability, std::vector<unsigned char> code) {
    this->lchild_ = NULL;
    this->rchild_ = NULL;
    this->symbol_ = symbol;
    this->probability_ = probability;
    this->code_ = code;

}

HNode::HNode(HNode* leftChild, HNode* rightChild) {
    this->lchild_ = leftChild;
    this->rchild_ = rightChild;
    this->probability_ = leftChild->probability() + rightChild->probability();
    this->code_ = std::vector<unsigned char>();
    this->lchild_->prependBit(1);
    this->rchild_->prependBit(0);
}

std::vector<HNode*> HNode::getTerminals() {
    std::vector<HNode*> res;
    if (this->lchild_ != NULL) {
        std::vector<HNode*> lterm = this->lchild_->getTerminals();
        res.reserve(res.size() + lterm.size());
        res.insert(res.end(), lterm.begin(), lterm.end());
    }
    if (this->rchild_ != NULL) {
        std::vector<HNode*> rterm = this->rchild_->getTerminals();
        res.reserve(res.size() + rterm.size());
        res.insert(res.end(), rterm.begin(), rterm.end());
    }

    if (this->isTerminal()) {
        res.push_back(this);
    }
    return res;
}

HNode::~HNode() {

    delete lchild_;
    lchild_ = NULL;
    delete rchild_;
    rchild_ = NULL;

}

double HNode::probability() {
    return this->probability_;
}

unsigned char HNode::symbol() {
    return this->symbol_;
}

bool HNode::isTerminal() {
    return (this->lchild_ == NULL && this->rchild_ == NULL);
}

void HNode::prependBit(short bitValue) {
    //this->code.push_back(bitValue);
    this->code_.insert(this->code_.begin(), bitValue);

    //propagate to children
    if (!isTerminal()) {
        this->lchild_->prependBit(bitValue);
        this->rchild_->prependBit(bitValue);
    }
}

std::vector< unsigned char> HNode::code()
{ 
  return std::vector<unsigned char>(this->code_);
}

/**
 * Number of bytes required for the code of the symbol
 */
unsigned char HNode::numBytes(){
  unsigned char count = 0x01;
  unsigned char s1 = this->code_.size();
  unsigned char s2 = (s1 >> 3)<<3; //divide and multiply by 8
  while(s1!=s2 && s1>0x08){
    s1 = s1 >> 3;
    s2 = s2 >> 3;
    count += 0x01;
    s2 = (s1 >> 3)<<3;
  }
  return count;
}

std::string HNode::codeString(){
  std::string res("");
  for(unsigned int i=0;i<this->code_.size();i++){
    unsigned char symbol = code_.at(i);
    
    if(symbol == 0x00){
      res.append("0");
    }
    if(symbol == 0x01){
      res.append("1");
    }
  }
  //fprintf(stderr,"\n");
  return res;
}


