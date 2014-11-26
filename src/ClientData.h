// ClientData.h
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



#ifndef INCLUDED_ClientData_H
#define INCLUDED_ClientData_H

namespace rude{
namespace cgiparser{

class ClientData{

protected:
	ClientData();

public:

	virtual void setCaseSensitive(bool iscase) = 0;

	// returns the total number of data elements that exist in the collection
	//
	virtual int numTotal() const = 0;

	// gets the identifying name of the data object at specified index
	//
	virtual const char *fieldAt(int index) const = 0;

	// there can be multiple values identified with the same fieldname,
	// this function returns the total number of values associated with
	// a particular fieldname
	//
	virtual int numValues(const char *fieldname) const = 0;

	// retrieve the nth value of the fieldname, where position determines
	// what the nth is. Use 0 for the first (and usually only) value.
	// Will never return NULL - returns "" if field or value does not exist.
	//
	virtual const char *value(int index) const = 0;
	virtual const char *value(const char *fieldname, int position) const = 0;

	// if the data object identified is a file, this will return the complete
	// path of the object as it exists on the clients remote computer
	// If the data object is not an uploaded file, or no filepath was specified,
	// then this will return NULL
	//
	virtual const char *filepath(int index) const = 0;
	virtual const char *filepath(const char *fieldname, int position) const = 0;

	// if the data object identified is a file, this will return the filename
	// of the object as it exists on the clients remote computer.
	// If the data object is not an uploaded file, or no filename was specified,
	// then this will return NULL.  The filename returned will be untainted,
	// and safe to use as a filename on the local machine. 
	//
	virtual const char *filename(int index) const = 0;
	virtual const char *filename(const char *fieldname, int position) const = 0;

	// If the data object identified by fieldname and position is a file,
	// then this will return the content-type associated with the file.
	// If no content-type was formally identified, this will return a 
	// defualt value of application/octet-stream.  If the object is not
	// a file object, this function will return Null
	//
	virtual const char *contentType(int index) const = 0;
	virtual const char *contentType(const char *fieldname, int position) const = 0;

	// Returns the length of the value identified by fieldname and position
	//
	virtual int length(int index) const = 0;
	virtual int length(const char *fieldname, int position) const = 0;

	virtual const char *datasource(int index) const = 0;
	virtual const char *datasource(const char *fieldname, int position) const = 0;

	// Was the field (empty or not) even submitted by the client?
	//
	virtual bool exists(const char *fieldname) const = 0;

	virtual ~ClientData();

};

}} // end namespaces

#endif

