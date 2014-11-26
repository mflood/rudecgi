// SimpleXMLParser.cpp
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




#include "SimpleXMLParser.h"

#include <stack>

#ifndef INCLUDED_DataRepos_H
#include "DataRepos.h"
#endif

#include "SimpleData.h"

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

namespace rude{
namespace cgiparser{

char *SimpleXMLParser::parseThingy( DataRepos *repository, char *buffer, int length)
{

		if(buffer[0] == '<')
		{
			if(buffer[1] == '/')
			{
				char *nextbuffer = strchr(buffer, '>');
				nextbuffer ++ ;
				parseThingy(repository, nextbuffer, length);
				return 0;

			}
			else
			{
				//start tag....
				//
				// find the tagname
				char *tagname = ++buffer;
				char *tagend = strchr(tagname, '>');
				if(tagend)
				{
					*tagend=0;
    				 tagend++;
    				buffer = tagend;
					SimpleData *data = new SimpleData();
					data->setFieldName(tagname);
					char *value = parseThingy(repository, buffer, length);
					data->setValue(value);
					repository->addDataObject(data);
				}
				return 0;
			}
		}
		else
		{
			char *end = strchr(buffer, '<');
			if(end)
			{
				*end = 0;
				end++;
				char *newbuffer = strchr(end, '<');
				if(newbuffer)
				{
					parseThingy(repository, newbuffer, length);
				}
			}
			return buffer;

		}
}






// NOTE: the original buffer gets mangled!!!
//
bool SimpleXMLParser::parse(DataRepos *repository, char *buffer, int length)
{

	
	if(buffer)
	{
		parseThingy(repository, buffer, length);
	}
	return true;
}

}} // end namespaces

