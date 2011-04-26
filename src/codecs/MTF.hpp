/*
    Move-To-Front implementation
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

#ifndef _MTF_H
#define	_MTF_H
#include <string>
#include "../defines.hpp"
#include "stdio.h"
#include "IEncoder.hpp"

class MTF: public IEncoder {
public:
    MTF(const std::string* infilename, void (*debug_ptr)(std::string, int), int verbose_value);
    MTF(const MTF& orig);
    virtual ~MTF();
    void encode(const std::string*outfile);
    void decode(const std::string*outfile);
private:
    void (*debug)(std::string, int);
    int verbose;
    std::string* filename;
};

#endif	/* _MTF_H */

