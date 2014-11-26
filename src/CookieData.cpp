// CookieData.cc
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



#include "CookieData.h"

namespace rude{
namespace cgiparser{

CookieData::CookieData()
{

}
	
CookieData::CookieData(const char *name, const char *value): SimpleData(name, value)
{

}

const char *CookieData::getContentType() const
{
	return "application/x-cookiedata";
}

const char *CookieData::datasource() const
{
	return "cookie";
}	

CookieData::~CookieData()
{

}
	
const char *CookieData::getFieldName() const
{
	return SimpleData::getFieldName();
}

	// returns the value if it exists, 
	// or the empty string
	//
const char *CookieData::getValue() const
{
	return SimpleData::getValue();
}

int CookieData::getLength() const
{
	return SimpleData::getLength();
}

const char *CookieData::getFilepath() const
{
	return SimpleData::getFilepath();
}

const char *CookieData::getFilename() const
{
	return SimpleData::getFilename();
}

void CookieData::setValue(const char *value)
{
	SimpleData::setValue(value);
}

void CookieData::setFieldName(const char *name)
{
	SimpleData::setFieldName(name);
}

}} // end namespaces

