// AbstractImplementation.cc
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



#include "AbstractImplementation.h"
#include "Implementation.h"

namespace rude{
namespace cgiparser{

AbstractImplementation *AbstractImplementation::s_instance=0;

bool AbstractImplementation::finished=false;

AbstractImplementation* AbstractImplementation::instance()
{
	if(!s_instance)
	{	
		if(finished)
		{
			return 0;
		}
		s_instance = new Implementation();
	}
	return s_instance;
}

void AbstractImplementation::finish()
{
	if(s_instance)
	{
		delete s_instance;
		s_instance=0;
		finished=true;
	}
}

AbstractImplementation::AbstractImplementation()
{

}

AbstractImplementation::~AbstractImplementation()
{

}

}} // end namespaces

