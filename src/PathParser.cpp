// PathParser.cpp
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




#include "PathParser.h"

#ifndef INCLUDED_DataRepos_H
#include "DataRepos.h"
#endif

#ifndef INCLUDED_PathData_H
#include "PathData.h"
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

#ifndef INCLUDED_CTYPE_H
#include <ctype.h>
#define INCLUDED_CTYPE_H
#endif

namespace rude{
namespace cgiparser{

char PathParser::s_delimiter = '=';

void PathParser::setDelimiter(char delimiter)
{
	s_delimiter = delimiter;
}

// Helper Method
//
void PathParser::addField(DataRepos *repository, const char *fieldstuff)
{
	if(!s_delimiter)
	{
		return;
	}
	
	int len=strlen(fieldstuff);
	if(len==0)
	{
		return;
	}
	char *buffer=new char[len+1];
	strcpy(buffer, fieldstuff);
	
	char *value=strchr(buffer, s_delimiter);
	if(value)
	{
		*value=(char)NULL;
		AbstractParser::unescape(++value);
	}
	AbstractParser::unescape(buffer);
	
	PathData *newData=new PathData(buffer, value);
	repository->addDataObject(newData);
	delete []buffer;
}


// NOTE: the original buffer gets mangled!!!
//
bool PathParser::parse(DataRepos *repository, char *buffer, int length)
{
	// cookie "name=value" must not contain whitespace, comma, or semi-colon,
	// so we can split on those things to get all cookies...
	//
	if(length)
	{
		// we have a length of at least 1, so this will always return something
		//

		char *key=strtok(buffer, "/");

		if(key)
		{
			addField(repository, key);

			// get more tokens
			//
			while((key=strtok(NULL, "/"))!=NULL)
			{
					addField(repository, key);
			}
		}
	}
	return true;
}

}} // end namespaces

