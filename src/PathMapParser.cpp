// PathMapParser.cpp
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



#include "PathMapParser.h"

#ifndef INCLUDED_DataRepos_H
#include "DataRepos.h"
#endif

#ifndef INCLUDED_PathData_H
#include "PathData.h"
#endif

#ifndef INCLUDED_PathMap_H
#include "PathMap.h"
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

namespace rude{
namespace cgiparser{
// NOTE: the original buffer gets mangled!!!
//
bool PathMapParser::parse(DataRepos *repository, char *buffer, int length)
{
	if(length)
	{
		// we have a length of at least 1, so this will always return something
		//

		PathMap *map = PathMap::instance();
		int numpathmaps = map->getNumPaths();
		int pathsprocessed = 0;

		// get the first token, the paths are set up as /token1/token2/token3/ etc...
		//
		char *key=strtok(buffer, "/");
		
		if(numpathmaps)
		{
			// if the token does not match "default" (or whatever default name is)
			//
			if(strcmp(key, map->getVoidName()) != 0)
			{
				// then figure out the pathmap name and add the value to the repository
				//
				const char *fieldname = map->getNameAt(pathsprocessed);
				AbstractParser::unescape(key);
				PathData *newData=new PathData(fieldname, key);
				repository->addDataObject(newData);
			}
			pathsprocessed++;
		}
	
		// get more tokens
		//
		while((key=strtok(NULL, "/"))!=NULL && pathsprocessed > numpathmaps)
		{
				if(strcmp(key, map->getVoidName())!=0)
				{
					const char *fieldname = map->getNameAt(pathsprocessed);
					AbstractParser::unescape(key);
					PathData *newData=new PathData(fieldname, key);
					repository->addDataObject(newData);
				}
				pathsprocessed++;
		}
	}
	return true;
}

}}

// $Log: PathMapParser.cpp,v $
// Revision 1.4  2003/06/22 23:44:34  cvs
// Added rude namespace
//
// Revision 1.3  2003/01/08 17:43:18  homeless
// added documentation
//
