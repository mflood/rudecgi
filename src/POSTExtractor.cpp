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

#ifndef INCLUDED_ERRNO_H
#include <errno.h>
#define INCLUDED_ERRNO_H
#endif

#ifndef INCLUDED_LIMITS_H
#include <limits.h>
#define INCLUDED_LIMITS_H
#endif

#ifndef INCLUDED_NEW_H
#include <new>
#define INCLUDED_NEW_H
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

	// See if there is even any posted data.
	//
	// CONTENT_LENGTH is absent for a GET, for a POST with no body, and
	// whenever this program is run from a shell.  getenv() then returns
	// NULL and atoi(NULL) is undefined behaviour - in practice a segfault.
	// It is also entirely attacker-influenced, so it may be negative, not
	// a number at all, or larger than any body we are willing to hold.
	//
	const char *contentlength = getenv("CONTENT_LENGTH");
	long length = 0;

	if(contentlength && *contentlength)
	{
		char *endptr = 0;
		errno = 0;
		long parsed = strtol(contentlength, &endptr, 10);

		// Reject trailing garbage, overflow and negative lengths rather
		// than silently treating them as some other number.
		//
		if(endptr != contentlength && *endptr == 0 && errno != ERANGE
		   && parsed > 0 && parsed <= (long) INT_MAX)
		{
			length = parsed;
		}
	}

	// Apply the caller's cap before allocating, not after: the whole point
	// of maxPostLength() is to bound what a request can make us allocate.
	//
	if(MAXPOSTLENGTH > 0 && length >= MAXPOSTLENGTH)
	{
		length = 0;
	}

	// also make sure we have a factory to use
	//
	if(length > 0 && d_factory)
	{

		// get the appropriate parser for the given content type
		//
		AbstractParser *parser = d_factory->buildParser(getenv("CONTENT_TYPE"));

		// read the incoming data into a buffer
		//
		// Zero-filled: a short read must not leave uninitialised heap in
		// the tail of the buffer, where it would be parsed and handed back
		// to the caller as field values.
		//
		// nothrow: an unbounded CONTENT_LENGTH must not turn into a
		// std::bad_alloc that terminates the whole CGI process.
		//
		char *buffer = new (std::nothrow) char[length + 1]();
		if(!buffer)
		{
			delete parser;
			return repository;
		}

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
			// Trust what we actually read, not what the request claimed.
			// An over-declared CONTENT_LENGTH used to leave the unread tail
			// of the buffer uninitialised and parse it anyway, disclosing
			// heap contents - including the buffers GETExtractor just freed
			// - to whoever sent the request.
			//
			size_t bytesread = fread(buffer, sizeof(char), (size_t) length, stdin);
			length = (long) bytesread;
			buffer[length] = (char) 0;
			
			// if content was encoded (eg. base64 / quoted-print / gzip) it would bedecoded now
			/////////////


			// parse the data, add it to repository
			//
			parser->parse(repository, buffer, (int) length);

		}
		else
		{
			// parse the data, add it to repository
			//
			char errormessage[] = "searchfor=binaryerror";
			parser->parse(repository, errormessage, strlen(errormessage));
		}
		// free unused objects
		//
		delete parser;
		delete [] buffer;
	}
	return repository;
}

}} // end namespaces

