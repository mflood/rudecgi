// Factory.cpp
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



#include "Factory.h"

#ifndef INCLUDED_CommandLineExtractor_H
#include "CommandLineExtractor.h"
#endif

#ifndef INCLUDED_GETExtractor_H
#include "GETExtractor.h"
#endif

#ifndef INCLUDED_POSTExtractor_H
#include "POSTExtractor.h"
#endif

#ifndef INCLUDED_REQ_UNKOWNEXTR_H
#include "UnknownExtractor.h"
#endif

#ifndef INCLUDED_ParserFactory_H
#include "ParserFactory.h"
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

namespace rude{
namespace cgiparser{

FormdataExtractor * Factory::buildExtractor(const char *method) const
{
	if(method)
	{
		if(strcmp(method, "GET") == 0)
		{
			return new GETExtractor();
			
		}
		else if(strcmp(method, "POST") == 0)
		{
			return (new POSTExtractor(new ParserFactory()));
		}
		else if(strcmp(method, "") == 0)
		{
			return new CommandLineExtractor();
		}
	}
	else
	{
		return new CommandLineExtractor();
	}
	return new UnknownExtractor();
}

}} // end namespaces

