// POSTExtractor.h
//
// Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005 Matthew Flood
// See file AUTHORS for contact information
//
// This file is part of RudeCGI.
//
// RudeCGI is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
// 
// RudeCGI is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with RudeCGI; (see COPYING) if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.
//------------------------------------------------------------------------




#ifndef INCLUDED_POSTExtractor_H
#define INCLUDED_POSTExtractor_H

#ifndef INCLUDED_FormdataExtractor_H
#include "FormdataExtractor.h"
#endif

namespace rude{
namespace cgiparser{


class ClientData;
class AbstractParserFactory;

class POSTExtractor: public FormdataExtractor{

	AbstractParserFactory *d_factory;

public:

		static long MAXPOSTLENGTH;

		POSTExtractor(AbstractParserFactory *factory);

		ClientData *extract();

		~POSTExtractor();

};

}} // end namespaces

#endif

