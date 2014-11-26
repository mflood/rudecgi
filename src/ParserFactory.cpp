// LiteParserFactory.cpp
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




#include "ParserFactory.h"

#ifndef INCLUDED_URLEncodeParser_H
#include "URLEncodeParser.h"
#endif

//#ifndef INCLUDED_MultipartParser_H
//#include "MultipartParser.h"
//#endif

#include "NewMultipartParser.h"

#ifndef INCLUDED_UnknownParser_H
#include "UnknownParser.h"
#endif

#ifndef INCLUDED_SimpleXMLParser_H
#include "SimpleXMLParser.h"
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

namespace rude{
namespace cgiparser{
AbstractParser * ParserFactory::buildParser(const char *contenttype)
{
	if(contenttype)
	{
		if( strcmp( contenttype, "application/x-www-form-urlencoded") == 0)
		{
			return new URLEncodeParser();
		}
		else if(  strstr(contenttype, "multipart/form-data"))
		{
			//const char *boundaryequals;
			// find boundary keyword
			//
			const char *boundaryword = strstr(contenttype, "boundary=");

			if(boundaryword)
			{
				// advance to equals sign
				//
				const char *boundary=strchr(boundaryword, '=');
						
				if(boundary != NULL)
				{
					// advance past equals
					//
					boundary=boundary+1;
					
					//MultipartParser *parser = new MultipartParser(boundary);
					NewMultipartParser *parser = new NewMultipartParser(boundary);
					return parser;

				}
			}
		}
		else if( strcmp( contenttype, "text/xml") == 0)
		{
			return new SimpleXMLParser();
		}
	}
	return new UnknownParser(contenttype);
}
}}
