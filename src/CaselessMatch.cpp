// CaselessMatch.cpp
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




#include "CaselessMatch.h"

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

#ifndef INCLUDED_CTYPE_H
#include <ctype.h>
#define INCLUDED_CTYPE_H
#endif

namespace rude{
namespace cgiparser{

bool CaselessMatch::compare(const char *string1, const char *string2) const
{
	if(string1 && string2)
	{
                int length1=strlen(string1);
                int length2=strlen(string2);

					 int testlength;
					 if(length1 < length2)
					 {
						testlength=length1;
					 }
					 else
					 {
						testlength=length2;
					 }

					for(int x=0; x < testlength; x++)
               {
                                char a=string1[x];
                                char b=string2[x];
                                if(isupper(a))
                                {
                                        a=tolower(a);
                                }
                                if(isupper(b))
                                {
                                        b=tolower(b);
                                }
                                if(a < b)
                                {
										return false;
                                        //return -1;
                                }
                                if(a > b)
                                {
										return false;
                                        //return 1;
                                }
                }
				

					if(length1 == length2)
					{
						return true;
						//return 0;
					}
					return false;
	}
	else
	{
		// return true if both words are null,
		// false if only one is null
		//
		return !( string1 || string2);
	}
}

}} // end namespaces

