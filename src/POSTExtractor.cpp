// POSTExtractor.cpp
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




#include "POSTExtractor.h"

#ifndef INCLUDED_GETExtractor_H
#include "GETExtractor.h"
#endif

#ifndef INCLUDED_AbstractParser_H
#include "AbstractParser.h"
#endif

#ifndef INCUDED_REQ_ABSTRACTFACTORY_H
#include "AbstractParserFactory.h"
#endif

#ifndef INCLUDED_DataRepos_H
#include "DataRepos.h"
#endif

#ifndef INCLUDED_STDLIB_H
#include <stdlib.h>
#define INCLUDED_STDLIB_H
#endif

#ifndef INCLUDED_STDIO_H
#include <stdio.h>
#define INCLUDED_STDIO_H
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

#ifdef WIN32
#include <io.h>
#endif

#include <fcntl.h>

namespace rude{
namespace cgiparser{

long POSTExtractor::MAXPOSTLENGTH=0;

POSTExtractor::POSTExtractor(AbstractParserFactory *factory)
{
	d_factory = factory;
}

POSTExtractor::~POSTExtractor()
{
	delete d_factory;
}

ClientData *POSTExtractor::extract()
{
	// First get query string data if there is any
	// and get the repository that the getExtractor builds
	//
	GETExtractor getExtractor;
	DataRepos *repository = getExtractor.extractDataRepos();

	// see if there is even any posted data
	//
	int length = atoi(getenv("CONTENT_LENGTH"));

	// also make sure we have a factory to use
	//
	if(length > 0 && d_factory && (MAXPOSTLENGTH == 0 || length < MAXPOSTLENGTH))
	{

		// get the appropriate parser for the given content type
		//
		AbstractParser *parser = d_factory->buildParser(getenv("CONTENT_TYPE"));

		// read the incoming data into a buffer
		//
		char *buffer = new char[length + 1];
		buffer[0] = 0;

#ifdef WIN32
		int result = setmode( fileno( stdin ), O_BINARY );
		if(result != -1)
#else
		int stdin_fd = fileno(stdin);
		//fclose(stdin);
		stdin = fdopen(stdin_fd, "rb"); 
		if(stdin)
#endif
		{
			fread(buffer, sizeof(char), length,stdin);
			buffer[length] = (char) 0;
			
			// if content was encoded (eg. base64 / quoted-print / gzip) it would bedecoded now
			/////////////


			// parse the data, add it to repository
			//
			parser->parse(repository, buffer, length);

		}
		else
		{
			// parse the data, add it to repository
			//
			parser->parse(repository, "searchfor=binaryerror", strlen("searchfor=binaryerror"));
		}
		// free unused objects
		//
		delete parser;
		delete [] buffer;
	}
	return repository;
}

}} // end namespaces

