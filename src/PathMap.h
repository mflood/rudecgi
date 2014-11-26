//PathMap.h
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


#ifndef INCLUDED_PathMap_H
#define INCLUDED_PathMap_H

#ifndef INCLUDED_VECTOR
#include <vector>
#define INCLUDED_VECTOR
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif
namespace rude{
namespace cgiparser{

class PathMap{

	std::string d_emptyname;
	std::vector<std::string> d_vector;
	static PathMap *s_instance;

protected:

	PathMap();

public:

	static PathMap *instance();
	int getNumPaths();
	const char *getNameAt(int index);
	void addName(const char *fieldname);
	void clear();
	void setVoidName(const char *emptyname);
	const char *getVoidName();

};

}} // end namespaces

#endif

