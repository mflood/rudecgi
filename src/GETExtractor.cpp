// GETExtractor.cpp
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




#include "GETExtractor.h"

#ifndef INCLUDED_CookieParser_H
#include "CookieParser.h"
#endif

#ifndef INCLUDED_PathParser_H
#include "PathParser.h"
#endif

#ifndef INCLUDED_PathMapParser_H
#include "PathMapParser.h"
#endif

#ifndef INCLUDED_REQDATAREPOS_H
#include "DataRepos.h"
#endif

#ifndef INCLUDED_URLEncodeParser_H
#include "URLEncodeParser.h"
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

#include <iostream>

#ifndef INCLUDED_STDLIB_H
#include <stdlib.h>
#define INCLUDED_STDLIB_H
#endif

namespace rude{
namespace cgiparser{

bool GETExtractor::PARSECOOKIES = true;
bool GETExtractor::PARSEPATH = true;
bool GETExtractor::PARSEPATHMAP = false;

DataRepos *GETExtractor::extractDataRepos()
{
	DataRepos *repository = new DataRepos();
	
	// Parse the Query String
	//
	char  *querystring=getenv("QUERY_STRING");
	if(querystring != NULL)
	{
		URLEncodeParser parser;

		int length = strlen(querystring);

		// the parser mangles input, so copy querystring into temp buffer
		//
		char *buffer = new char[length+1];
		strcpy(buffer, querystring);
		
		parser.parse(repository, buffer, length);

		delete [] buffer;
	}

	
	// Parse Path Data
	//
	char  *pathstring=getenv("PATH_INFO");
	if(pathstring != NULL)
	{
		// the parser mangles input, so copy pathstring into temp buffer
		//
		int length = strlen(pathstring);
		char *buffer = new char[length+1];

		if(PARSEPATH)
		{
			PathParser parser;
			strcpy(buffer, pathstring);
			parser.parse(repository, buffer, length);
		}
		
		if(PARSEPATHMAP)
		{
			PathMapParser parser;
			strcpy(buffer, pathstring);
			parser.parse(repository, buffer, length);
		}
		delete [] buffer;
	}

	// Parse Cookies
	//
	char  *cookiestring=getenv("HTTP_COOKIE");
	if(cookiestring != NULL && PARSECOOKIES)
	{
		CookieParser parser;

		int length = strlen(cookiestring);

		// the parser mangles input, so copy querystring into temp buffer
		//
		char *buffer = new char[length+1];
		strcpy(buffer, cookiestring);
		
		parser.parse(repository, buffer, length);

		delete [] buffer;
	}


	
	return repository;
}

ClientData *GETExtractor::extract()
{
	return extractDataRepos();
}

}} // end namespaces

