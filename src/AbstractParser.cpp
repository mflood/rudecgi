// AbstractParser.cpp
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




#include "AbstractParser.h"

#ifndef INCLUDED_CTYPE_H
#include <ctype.h>
#define INCLUDED_CTYPE_H
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

namespace rude{
namespace cgiparser{

AbstractParser::AbstractParser()
{

}

AbstractParser::~AbstractParser()
{

}
	
void AbstractParser::setError(const char *error)
{
	d_error = error? error: "";
}

const char *AbstractParser::getError()
{
	return d_error.c_str();
}

//=
//  Unescapes a URL Encoded string.
//  @return 0 for success, 1 if there was an illegal escape sequence (non hex value).
//=

int AbstractParser::unescape(char *s)
{
	int Error=0;
	static  char *Hex="0123456789ABCDEF";
	unsigned int Ascii;
	char *p;
	char *Match;
	for(p=s ; !Error && *s != '\0'; s++)
	{
		if(*s == '%')
		{
			s++;
			if(*s=='\0')
			{
				break;
			}
			if(islower(*s))
			{
				*s=toupper(*s);
			}
			if((Match=strchr(Hex, *s)) != NULL)
			{
				Ascii=(unsigned int)(Match - Hex);
				s++;
				if(*s=='\0')
				{
					break;
				}
				if(islower(*s))
				{
					*s=toupper(*s);
				}
				if( (Match = strchr(Hex, *s)) != NULL)
				{
					Ascii<<=4;
					Ascii |= (unsigned int)(Match - Hex);
					*p++ = (char)Ascii;
				}
				else
				{
					Error=1;
					continue;
				}
			}
			else
			{
				Error=1;
				continue;
			}
		}
		else if(*s == '+')
		{
			*p++=' ';
		}
		else
		{
			*p++=*s;
		}
	}
	*p='\0';
	return Error;
}

}} // end namespaces

