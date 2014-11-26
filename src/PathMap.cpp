//PathMap.cc
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



#include "PathMap.h"

namespace rude{
namespace cgiparser{

PathMap *PathMap::s_instance=0;

PathMap::PathMap()
{
	d_emptyname="default";
}

PathMap *PathMap::instance()
{
	if(!s_instance)
	{
		s_instance = new PathMap();
	}
	return s_instance;
}
void PathMap::setVoidName(const char *emptyname)
{
	d_emptyname=emptyname ? emptyname: "";
}
const char *PathMap::getVoidName()
{
	return d_emptyname.c_str();
}

int PathMap::getNumPaths()
{
	return d_vector.size();
}

	
const char *PathMap::getNameAt(int index)
{
	std::vector<std::string>::iterator iter;
	int x=0;
	for(iter=d_vector.begin(); iter != d_vector.end(); iter++)
	{
		if(x == index)
		{
			return (*iter).c_str();
		}
		x++;
	}
	return "";
}

void PathMap::addName(const char *fieldname)
{
	std::string newname = fieldname ? fieldname: "";
	d_vector.push_back(newname);
}
	
void PathMap::clear()
{
	d_vector.clear();
}
}}
