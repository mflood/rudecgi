// Implementation.cc
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



#include "Implementation.h"

#ifndef INCLUDED_Factory_H
#include "Factory.h"
#endif

#ifndef INCLUDED_ClientData_H
#include "ClientData.h"
#endif

#ifndef INCLUDED_FormdataExtractor_H
#include "FormdataExtractor.h"
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

#ifndef INCLUDED_STDLIB_H
#include <stdlib.h>
#define INCLUDED_STDLIB_H
#endif

namespace rude{
namespace cgiparser{
Implementation::Implementation()
{
	AbstractFactory *factory = new Factory();
	
	FormdataExtractor *extractor = factory->buildExtractor(getenv("REQUEST_METHOD"));

	d_clientdata = extractor->extract();

	delete extractor;
	delete factory;
}



Implementation::~Implementation()
{
	delete d_clientdata;
}


void Implementation::setCaseSensitive(bool isCaseSenstive) 
{
	d_clientdata->setCaseSensitive(isCaseSenstive);
}

int Implementation::numValues() const
{
	return d_clientdata->numTotal();
}
	
int Implementation::numValues(const char *fieldname) const
{
	return d_clientdata->numValues(fieldname);
}
	
const char *Implementation::fieldnameAt(int position) const
{
	return d_clientdata->fieldAt(position);
}
	
bool Implementation::exists(const char *fieldname) const
{
	return d_clientdata->exists(fieldname);
}
	
bool Implementation::isFile(const char *fieldname) const
{
	return (d_clientdata->filename(fieldname,0) != 0 && d_clientdata->filename(fieldname,0)[0] != 0);
}

bool Implementation::isFile(const char *fieldname, int position) const
{
	return (d_clientdata->filename(fieldname,position) != 0 && d_clientdata->filename(fieldname,0)[0] != 0);
}

const char *Implementation::datasource(const char *fieldname) const
{
	return d_clientdata->datasource(fieldname,0);
}

const char *Implementation::datasource(const char *fieldname, int position) const
{
	return d_clientdata->datasource(fieldname,position);
}

const char *Implementation::value( const char *fieldname) const
{
	const char *ret = d_clientdata->value(fieldname, 0);
	return ret;
}

const char * Implementation::value(const char *fieldname, int position) const 
{
	return d_clientdata->value(fieldname, position);
}

int Implementation::length(const char *fieldname) const 
{
	return d_clientdata->length(fieldname, 0);
}
	
int Implementation::length(const char *fieldname, int position) const 
{
	return d_clientdata->length(fieldname, position);
}
	
const char *Implementation::contenttype(const char *fieldname) const 
{
	return d_clientdata->contentType(fieldname, 0);
}
	
const char *Implementation::contenttype(const char *fieldname, int position) const 
{
	return d_clientdata->contentType(fieldname, position);
}
	
const char *Implementation::filename(const char *fieldname) const 
{
	return d_clientdata->filename(fieldname, 0);
}

const char *Implementation::filename(const char *fieldname, int position)  const 
{
	return d_clientdata->filename(fieldname, position);
}

const char *Implementation::filepath(const char *fieldname) const 
{
	return d_clientdata->filepath(fieldname, 0);
}

const char *Implementation::filepath(const char *fieldname, int position)  const 
{
	return d_clientdata->filepath(fieldname, position);
}

bool Implementation::isFile(int index) const 
{
	return (d_clientdata->filename(index) != 0 && d_clientdata->filename(index)[0] != 0);
}

const char *Implementation::datasource(int index) const 
{
	return d_clientdata->datasource(index);
}

const char *Implementation::value(int index) const 
{
	return d_clientdata->value(index);
}

int Implementation::length(int index) const 
{
	return d_clientdata->length(index);

}
	
const char *Implementation::contenttype(int index) const 
{
	return d_clientdata->contentType(index);

}

const char *Implementation::filename(int index) const 
{
	return d_clientdata->filename(index);

}
	
const char *Implementation::filepath(int index) const 
{
	return d_clientdata->filepath(index);

}

}} // end namespaces

