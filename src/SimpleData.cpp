// SimpleData.cc
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



#include "SimpleData.h"
namespace rude{
namespace cgiparser{

SimpleData::SimpleData()
{

}
	
SimpleData::SimpleData(const char *name, const char *value)
{
		d_name = name ? name: "";
		d_value = value ? value: "";
}

const char *SimpleData::getFieldName() const
{
	return d_name.c_str();
}

const char *SimpleData::getValue() const
{
	return d_value.c_str();
}

int SimpleData::getLength() const
{
	return d_value.length();
}

const char *SimpleData::getFilepath() const
{
	return 0;
}

const char *SimpleData::getFilename() const
{
	return 0;
}

const char *SimpleData::getContentType() const
{
	return "";
}

void SimpleData::setValue(const char *value)
{
	d_value = value? value: "";
}

void SimpleData::setFieldName(const char *name)
{
	d_name = name? name: "";
}

const char *SimpleData::datasource() const
{
	return "form";
}

SimpleData::~SimpleData()
{

}
}}
