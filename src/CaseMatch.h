// CaseMatch.h
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




#ifndef INCLUDED_CaseMatch_H
#define INCLUDED_CaseMatch_H

#ifndef INCLUDED_WordMatch_H
#include "WordMatch.h"
#endif

namespace rude{
namespace cgiparser{

//=
// Strategy Implementation (for {@link source.WordMatch WordMatch})- Uses a case-sensitive algorithm to compare words
//
//=
class CaseMatch: public WordMatch{

public:

	//=
	// performs a case sensitive compare
	//
	// @return <b>true</b> if the words match exactly, <b>false otherwise</b>
	//=
	bool compare(const char *word1, const char *word2) const;

};

}} // end namespaces

#endif

