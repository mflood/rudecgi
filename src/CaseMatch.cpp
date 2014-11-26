// CaseMatch.cpp
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




#include "CaseMatch.h"

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

namespace rude{
namespace cgiparser{

bool CaseMatch::compare(const char *word1, const char *word2) const
{
	if(word1 && word2)
	{
		// return true if they are equal
		//
		return (strcmp(word1, word2) == 0);
	}
	else
	{
		// return true if both words are null,
		// false if only one is null
		//
		return !( word1 || word2);
	}
}

}} // end namespaces

