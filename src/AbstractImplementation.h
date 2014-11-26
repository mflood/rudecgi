// AbstractImplementation.h
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



#ifndef INCLUDED_AbstractImplementation_H
#define INCLUDED_AbstractImplementation_H

namespace rude{
namespace cgiparser{

//=
// This class provides the implementation interface for the Req_Request Object.  It is based on the Bridge design pattern, 
// and facilitates changing the implementation of the Req_Request component without altering the public interface provided by the Req_Request facade.
// Every method of the Req_Request object is forwarded to this interface. As such, please refer to the {@link Req_Request Req_Request} object for documentation on
// how the methods are suppose to work, and see implementation classes to see variations in behavior.
//=
class AbstractImplementation{

	static AbstractImplementation *s_instance;
	static bool finished;

protected:

	AbstractImplementation();

public:
	
	static AbstractImplementation* instance();
	static void finish();
	virtual void setCaseSensitive(bool isCaseSenstive) = 0;

	virtual int numValues() const = 0;
	virtual int numValues(const char *fieldname) const = 0;
	
	virtual const char *fieldnameAt(int position) const = 0;
	virtual bool exists(const char *fieldname) const = 0;

	virtual bool isFile(int index) const = 0;
	virtual bool isFile(const char *fieldname) const = 0;
	virtual bool isFile(const char *fieldname, int position) const = 0;

	virtual const char *datasource(int position) const = 0;
	virtual const char *datasource(const char *fieldname) const = 0;
	virtual const char *datasource(const char *fieldname, int position) const = 0;

	virtual const char *value(int index) const = 0;
	virtual const char *value(const char *fieldname) const = 0;
	virtual const char *value(const char *fieldname, int position) const = 0;

	virtual int length(int index) const = 0;
	virtual int length(const char *fieldname) const = 0;
	virtual int length(const char *fieldname, int position) const = 0;
	
	virtual const char *contenttype(int index) const = 0;
	virtual const char *contenttype(const char *fieldname) const = 0;
	virtual const char *contenttype(const char *fieldname, int position) const = 0;

	virtual const char *filename(int index) const = 0;
	virtual const char *filename(const char *fieldname) const = 0;
	virtual const char *filename(const char *fieldname, int position) const = 0;
	
	virtual const char *filepath(int index) const = 0;
	virtual const char *filepath(const char *fieldname) const = 0;
	virtual const char *filepath(const char *fieldname, int position) const = 0;

	virtual ~AbstractImplementation();

};

}} // end namespaces

#endif

