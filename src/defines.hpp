/*
    Some definitions for debugging with general inline functions 
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
#include <string>
#include <sstream>
#include <cassert>
#ifndef _DEFINES_H
#define	_DEFINES_H

#define CRITICAL 0
#define INFO 1
#define DEBUG 2
#define DDEBUG 3


using namespace std;

static inline void null_debug(std::string , int ) {
  
}

static inline string makestring(unsigned counter) {
    stringstream s;
    s << counter;
    return s.str();
}

static inline string ucharToString(const unsigned char* input){
    stringstream s;
    s << (int)*input;

    return s.str();
}

static inline string ulongToString(const unsigned long* input){
    stringstream s;
    s << (long)*input;

    return s.str();
}


static inline void removeExtension(std::string* filename)
{
  const int dot_index = filename->rfind(".",filename->size());
  assert(dot_index != -1 && "No dot found in filename");
  std::string* old = filename;
  filename = new std::string(filename->substr(0,dot_index));
  delete old;
}

 

#endif	/* _DEFINES_H */

