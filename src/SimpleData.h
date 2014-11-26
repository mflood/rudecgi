// SimpleData.h
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



#ifndef INCLUDED_SimpleData_H
#define INCLUDED_SimpleData_H

#ifndef INCLUDED_DataObject_H
#include "DataObject.h"
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

namespace rude{
namespace cgiparser{

// this class is a liteweight hash table
// it expects values to be simple cstrings,
// not binary data. For binary data, use
// Req_FileObject.
//
class SimpleData: public DataObject
{

	// holds the fieldname
	//
	std::string d_name;

	// holds the string value
	//
	std::string d_value;

public:

	// CONSTRUCTORS
	//
	SimpleData();
	SimpleData(const char *name, const char *value);

	// DESTRUCTOR
	//
	~SimpleData();
	
	// returns the fieldname if it exists, 
	// or an empty string
	//
	const char *getFieldName() const;

	// returns the value if it exists, 
	// or the empty string
	//
	const char *getValue() const;

	// returns the length of the value,
	// or 0 if there is no value
	//
	int getLength() const;

	// since this is not a file object,
	// this will always return NULL
	//
	const char *getFilepath() const;

	// since this is not a file object, 
	// this will always return NULL
	//
	const char *getFilename() const;

	// since this is not a file object,
	// this will always return NULL
	//
	const char *getContentType() const;

	// sets the value of the data object
	// If value is NULL, the internal value will
	// be the empty string
	//
	void setValue(const char *value);

	// sets the identifying name of the data object
	// if name is NULL, the internal value will be the empty string
	//
	void setFieldName(const char *name);


	// always "form"
	const char *datasource() const;
};

}} // end namespaces

#endif

