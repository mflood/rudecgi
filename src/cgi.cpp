// CGI.cc
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



#include "cgi.h"

#ifndef INCLUDED_AbstractImplementation_H
#include "AbstractImplementation.h"
#endif

#ifndef INCLUDED_PathParser_H
#include "PathParser.h"
#endif

#ifndef INCLUDED_PathMap_H
#include "PathMap.h"
#endif

#ifndef INCLUDED_GETExtractor_H
#include "GETExtractor.h"
#endif

#ifndef INCLUDED_POSTExtractor_H
#include "POSTExtractor.h"
#endif

using namespace rude::cgiparser;

namespace rude{

std::ostream& operator<<(std::ostream& os, const CGI& parser)
{
	int total=parser.numValues();

	for(int x=0; x< total; x++)
	{
		os << parser.fieldnameAt(x) << "=" << parser.value(x) << "\n";
	}
	return os;
}


AbstractImplementation *CGI::s_implementation=0;

const char *CGI::version()
{
	return "RudeServer CGI Version 3.0";
}
	

CGI::CGI()
{
	s_implementation = AbstractImplementation::instance();
}


void CGI::parsePathMap(bool shouldParse)
{
	GETExtractor::PARSEPATHMAP = shouldParse;
}

void CGI::parsePath(bool shouldParse)
{
	GETExtractor::PARSEPATHMAP = shouldParse;
}

void CGI::parseCookies(bool shouldParse)
{
	GETExtractor::PARSECOOKIES = shouldParse;
}

void CGI::maxPostLength(long bytes)
{
	if(bytes > 0)
	{
		POSTExtractor::MAXPOSTLENGTH=bytes;
	}
	else
	{
		POSTExtractor::MAXPOSTLENGTH=0;
	}
}

void CGI::setPathDelimiter(char delimiter)
{
	PathParser::setDelimiter(delimiter);
}
void CGI::setPathMapVoid(const char *emptyname)
{
	PathMap *map = PathMap::instance();
	map->setVoidName(emptyname);
}
void CGI::addPathMapName(const char *pathname)
{
	if(pathname)
	{
		PathMap *map = PathMap::instance();
		map->addName(pathname);
	}
}
void CGI::setCaseSensitive(bool isCaseSenstive)
{
	if(s_implementation)
	{
		s_implementation->setCaseSensitive(isCaseSenstive);
	}
}

const char *CGI::value( const char *fieldname) const
{
	return s_implementation ? s_implementation->value(fieldname) : "";
}
	
bool CGI::exists( const char *fieldname) const
{
	return s_implementation ? s_implementation->exists(fieldname) : false;
}

bool CGI::isFile(const char *fieldname) const
{
	return s_implementation ? s_implementation->isFile(fieldname) : false;
}

bool CGI::isFile(const char *fieldname, int position) const
{
	return s_implementation ? s_implementation->isFile(fieldname, position): false;
}

int CGI::numValues() const
{
	return s_implementation ? s_implementation->numValues(): 0;
}
	
int CGI::numValues(const char *fieldname) const
{
	return s_implementation ? s_implementation->numValues(fieldname) : 0;
}

const char *CGI::fieldnameAt(int position) const
{
	return s_implementation ? s_implementation->fieldnameAt(position): "";
}


const char * CGI::operator [] (const char * fieldname)
{
		return s_implementation ? s_implementation->value(fieldname, 0): "";
}

const char * CGI::operator [] (int x)
{
	return s_implementation ? s_implementation->fieldnameAt(x) : "";
}


const char * CGI::value(const char *fieldname, int position) const
{
	return s_implementation ? s_implementation->value(fieldname, position) : "";
}

int CGI::length(const char *fieldname) const
{
	return s_implementation ? s_implementation->length(fieldname) : 0;
}

int CGI::length(const char *fieldname, int position) const
{
	return s_implementation ? s_implementation->length(fieldname, position): 0;
}

const char *CGI::contenttype(const char *fieldname) const
{
	return s_implementation ? s_implementation->contenttype(fieldname): "";
}

const char *CGI::contenttype(const char *fieldname, int position) const
{
	return s_implementation ? s_implementation->contenttype(fieldname, position): "";
}

const char *CGI::filename(const char *fieldname) const
{
	return s_implementation ? s_implementation->filename(fieldname): "";
}

const char *CGI::filename(const char *fieldname, int position) const
{
	return s_implementation ? s_implementation->filename(fieldname, position): "";
}

const char *CGI::filepath(const char *fieldname) const
{
	return s_implementation ? s_implementation->filepath(fieldname) : "";
}

const char *CGI::filepath(const char *fieldname, int position) const
{
	return s_implementation ? s_implementation->filepath(fieldname, position) : "";
}

void CGI::finish()
{
	AbstractImplementation::finish();
	s_implementation=0;
}

CGI::~CGI()
{
	
}

const char *CGI::datasource(int position) const
{
	return s_implementation ? s_implementation->datasource(position): "";
}

const char *CGI::datasource(const char *fieldname) const
{
	return s_implementation ? s_implementation->datasource(fieldname): "";
}

const char *CGI::datasource(const char *fieldname, int position) const
{
	return s_implementation ? s_implementation->datasource(fieldname, position): "";
}

bool CGI::isFile(int index) const
{
	return s_implementation ? s_implementation->isFile(index) : false;
}

const char *CGI::value(int index) const
{
	return s_implementation ? s_implementation->value(index) : "";
}

int CGI::length(int index) const
{
	return s_implementation ? s_implementation->length(index) : 0;
}

const char *CGI::contenttype(int index) const
{
	return s_implementation ? s_implementation->contenttype(index) : "";
}

const char *CGI::filename(int index) const
{
	return s_implementation ? s_implementation->filename(index) : "";
}

const char *CGI::filepath(int index) const
{
	return s_implementation ? s_implementation->filepath(index): "";
}

} // end namespace rude


