// DataObject.h
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




#ifndef INCLUDED_DataObject_H
#define INCLUDED_DataObject_H

namespace rude{
namespace cgiparser{

//=
// Provides an interface to get information about a single piece of sent data
//=
class DataObject{

protected:
	//= 
	// This is an abstract class and cannot be instantiated
	//=
	DataObject();

public:
	//=
	// Returns the identifying field name of the data
	//=
	virtual const char *getFieldName() const = 0;
	
	//=
	// Returns the value associated with the field, or the file data for an uploaded file
	//=
	virtual const char *getValue() const = 0;
	
	//=
	// Returns the length of the Value
	//=
	virtual int getLength() const = 0;
	
	//=
	// For uploaded files, returns the original filepath on the client's machine
	//=
	virtual const char *getFilepath() const = 0;
	
	//=
	// For an uploaded file, returns a safe filename
	//=
	virtual const char *getFilename() const = 0;
	
	//=
	// For uploaded files, returns the content type of the file
	//=
	virtual const char *getContentType() const = 0;
	
	//=
	// Returns the source of the data - either "form", "cookie", or "path"
	//=
	virtual const char *datasource() const =0;

	virtual ~DataObject();
};

}}

#endif

