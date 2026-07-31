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

#ifndef INCLUDED_CHRONO
#include <chrono>
#define INCLUDED_CHRONO
#endif

#ifndef WIN32
#ifndef INCLUDED_SYS_SELECT_H
#include <sys/select.h>
#define INCLUDED_SYS_SELECT_H
#endif
#ifndef INCLUDED_UNISTD_H
#include <unistd.h>
#define INCLUDED_UNISTD_H
#endif
#endif

#ifdef WIN32
#include <io.h>
#endif

#include <fcntl.h>

namespace rude
{
namespace cgiparser
{

long POSTExtractor::MAXPOSTLENGTH = 0;
long POSTExtractor::MAXPOSTREADSECONDS = 0;

POSTExtractor::POSTExtractor(AbstractParserFactory *factory)
{
	d_factory = factory;
}

POSTExtractor::~POSTExtractor()
{
	delete d_factory;
}

//
// How long the body read will sit with nothing arriving at all before it
// gives up, when the caller has not set an overall limit of their own.
//
// This exists because the alternative default was "wait forever".  fread()
// returns only once it has the full count or sees EOF, so a client that
// over-declares CONTENT_LENGTH and then holds the connection open pinned the
// CGI process for as long as it cared to - no code change required on the
// client's part, and nothing in a default build to stop it.
//
// It is an *idle* limit rather than a cap on the whole read.  A large upload
// over a slow link legitimately takes minutes while making steady progress; a
// stall is the absence of progress, so that is what gets measured.  Any byte
// arriving restarts the clock.  The cost of that choice is that a client
// drip-feeding one byte every 59 seconds still gets to hang around - see
// CGI::maxPostReadSeconds(), which bounds the read as a whole.
//
static const long DEFAULT_IDLE_READ_SECONDS = 60;

//
// Reads up to 'length' bytes of request body.
//
// stdio is bypassed in favour of the raw descriptor so that select() can bound
// the wait.  Nothing has been read from stdin before this point, so switching
// to read() here cannot strand buffered data.
//
// timeoutsecs is CGI::maxPostReadSeconds():
//
//    > 0   bound the read as a whole; stop at that deadline.
//    == 0  the default - no overall bound, but stop after
//          DEFAULT_IDLE_READ_SECONDS with nothing received.
//    < 0   no bound of any kind; block in fread() until the body arrives or
//          the peer goes away.
//
// Whatever has arrived when the read stops is what gets parsed.
//
// select() over a pipe is POSIX.  On Windows it applies to sockets only, so
// none of this is available there and the read stays a plain fread().
//
static size_t readRequestBody(char *buffer, long length, long timeoutsecs)
{
#ifndef WIN32
	if(timeoutsecs >= 0)
	{
		int fd = fileno(stdin);
		if(fd >= 0)
		{
			const bool overall = (timeoutsecs > 0);
			const std::chrono::steady_clock::time_point deadline =
				std::chrono::steady_clock::now() + std::chrono::seconds(overall ? timeoutsecs : 0);

			size_t total = 0;
			while(total < (size_t) length)
			{
				// Wait out the idle allowance, or what is left of the
				// caller's overall deadline, whichever runs out first.
				//
				std::chrono::steady_clock::duration wait =
					std::chrono::seconds(DEFAULT_IDLE_READ_SECONDS);

				if(overall)
				{
					std::chrono::steady_clock::duration left =
						deadline - std::chrono::steady_clock::now();
					if(left.count() <= 0)
					{
						// Out of time: keep whatever arrived and parse that.
						//
						break;
					}
					wait = left;
				}

				long usecs = (long) std::chrono::duration_cast<
								 std::chrono::microseconds>(wait)
								 .count();
				struct timeval tv;
				tv.tv_sec = usecs / 1000000;
				tv.tv_usec = usecs % 1000000;

				fd_set fds;
				FD_ZERO(&fds);
				FD_SET(fd, &fds);

				int rc = select(fd + 1, &fds, (fd_set *) 0, (fd_set *) 0, &tv);
				if(rc <= 0)
				{
					// Timed out, or the descriptor went bad.
					//
					break;
				}

				ssize_t got = read(fd, buffer + total, (size_t) length - total);
				if(got <= 0)
				{
					// EOF, or an unrecoverable error.
					//
					break;
				}
				total += (size_t) got;

				// Progress: the idle allowance starts over.  Nothing to reset
				// explicitly, since it is measured per select() call.
			}
			return total;
		}
	}
#else
	(void) timeoutsecs;
#endif

	return fread(buffer, sizeof(char), (size_t) length, stdin);
}

ClientData *POSTExtractor::extract()
{
	// First get query string data if there is any
	// and get the repository that the getExtractor builds
	//
	// Cookies are deliberately left out here and applied at the end, after
	// the POST body.  Parsing them up front put client-controlled cookies
	// ahead of form fields in the shared namespace, so a cookie could
	// shadow a POST field at any unindexed value() call - the opposite of
	// what happens on a GET, where the query string is parsed first.
	//
	GETExtractor getExtractor;
	DataRepos *repository = getExtractor.extractDataReposWithoutCookies();

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
		if(endptr != contentlength && *endptr == 0 && errno != ERANGE && parsed > 0 && parsed <= (long) INT_MAX)
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
		char *buffer = new(std::nothrow) char[length + 1]();
		if(!buffer)
		{
			delete parser;
			GETExtractor::parseCookiesInto(repository);
			return repository;
		}

#ifdef WIN32
		int result = setmode(fileno(stdin), O_BINARY);
		if(result != -1)
#else
		int stdin_fd = fileno(stdin);
		// fclose(stdin);
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
			size_t bytesread = readRequestBody(buffer, length, MAXPOSTREADSECONDS);
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
		delete[] buffer;
	}

	// Cookies last, so form data wins a name collision on POST just as
	// it does on GET.
	//
	GETExtractor::parseCookiesInto(repository);

	return repository;
}

} // namespace cgiparser
} // namespace rude
