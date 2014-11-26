// PathData.h
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



#ifndef INCLUDED_PathData_H
#define INCLUDED_PathData_H

#ifndef INCLUDED_SimpleData_H
#include "SimpleData.h"
#endif

namespace rude{
namespace cgiparser{


class PathData: public SimpleData
{

public:

	// CONSTRUCTORS
	//
	PathData();
	PathData(const char *name, const char *value);

	// DESTRUCTOR
	//
	~PathData();

	//=
	// Returns the fieldname if it exists, or the empty string
	//=
	const char *getFieldName() const;

	//=
	// Returns the value if it exists, or the empty string
	//=
	const char *getValue() const;

	//=
	// Returns the length of the value,
	// or 0 if there is no value
	//=
	int getLength() const;

	//=
	// Since this is not a file object, this will always return NULL
	//=
	const char *getFilepath() const;

	//=
	// Since this is not a file object, this will always return NULL
	//=
	const char *getFilename() const;

	//=
	// This will always return application/x-pathdata
	//=
	const char *getContentType() const;

	//=
	// Used by the parser to set the value of the data object.
	// If value is NULL, the internal value will be the empty string
	//=
	void setValue(const char *value);

	//=
	// Used by the parser to set the identifying name of the data object.
	// If name is NULL, the internal value will be the empty string
	//=
	void setFieldName(const char *name);

	//=
	// Always returns "path"
	//=
	const char *datasource() const;
};
}}

#endif

